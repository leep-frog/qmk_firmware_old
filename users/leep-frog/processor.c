#ifndef LEEP_PROCESSOR_C
#define LEEP_PROCESSOR_C

#include "url.c"
#include "enum.c"

// #define LEEP_DEBUG 3

#ifdef LEEP_DEBUG
    #define LEBUG_STMT(x) x
    #define LEBUG_ARG(x) x,
#else
    #define LEBUG_STMT(x)
    #define LEBUG_ARG(x)
#endif
#define SIMPLE_PROCESSOR(name, f) _simple_processor(LEBUG_ARG(name) f)
#define SIMPLE_STRING_PROCESSOR(name, f, str) _simple_string_processor(LEBUG_ARG(name) f, str)
#define SIMPLE_BOOL_PROCESSOR(name, f, b) _simple_bool_processor(LEBUG_ARG(name) f, b)
#define STRING_SENDER(name, val) _string_sender(LEBUG_ARG(name) val)
#define URL_SENDER(name, val) _url_sender(LEBUG_ARG(name) val)
#define NEW_TAB_SENDER(name, val) _new_tab_sender(LEBUG_ARG(name) val)

uint16_t oom_error = false;

struct Processor {
    LEBUG_STMT(char* name;)
    union {
        char* send_string_arg;
        bool bool_arg;
    } data;
    struct Processor* next;
    bool (*f)(struct Processor*);
};

struct Processor* NewProcessor(void) {
    // TODO: check that we have a good amount of buffer memory
    // like if !malloc(size + N) {error} else {free(...)}
    // Actually, might not really be necessary since newkeynode is called
    // deep in the stack so a lot of memory will be freed up once that's done
    struct Processor* p = malloc(sizeof(struct Processor));
    if (!p) {
        oom_error = true;
        return NULL;
    }
    p->data.send_string_arg = NULL;
    p->next = NULL;
    p->f = NULL;
    LEBUG_STMT(p->name = NULL;)
    return p;
}

struct Processor* append_processor(struct Processor* p1, struct Processor* p2) {
    if (!p1) {
        return p2;
    }
    struct Processor* cur = p1;
    for (; cur->next; cur = cur->next) {}
    cur->next = p2;
    return p1;
}

struct KeyNode {
    uint16_t a;
    uint16_t b;
    struct KeyNode* left;
    struct KeyNode* middle;
    struct KeyNode* right;

    struct Processor* a_processor;
    struct Processor* b_processor;
};

bool is_three_node(struct KeyNode* kn) {
    return (kn->b);
}

bool is_two_node(struct KeyNode* kn) {
    return !is_three_node(kn);
}

void to_two_node(struct KeyNode* kn) {
    kn->b = 0;
    kn->b_processor = NULL;
    kn->right = NULL;
}

struct KeyNode* NewKeyNode(uint16_t idx, struct Processor* p) {
    struct KeyNode* kn = malloc(sizeof(struct KeyNode));
    if (!kn) {
        oom_error = true;
        return NULL;
    }
    kn->a = idx;
    kn->a_processor = p;

    kn->b = 0;
    kn->b_processor = NULL;

    kn->left = NULL;
    kn->middle = NULL;
    kn->right = NULL;
    return kn;
}

void print_int(int i) {
    char c[10];
    itoa(i, c, 10);
    send_string(c);
}

#ifdef LEEP_DEBUG
void print_depth(uint16_t depth) {
    for (uint16_t i = 0;i < depth; i++) {
        send_string("  ");
    }
}

void print_tree(struct KeyNode* kn, uint16_t depth) {
    if (!kn) {
        return;
    }
    print_tree(kn->left, depth+1);
    print_depth(depth);
    send_string("a: ");
    print_int(kn->a);
    send_string(": ");
    for (struct Processor* cur = kn->a_processor; cur; cur = cur->next) {
        if (cur->name) {
            send_string(cur->name);
        } else {
            send_string("no_name");
        }
        send_string(", ");
    }
    send_string("\n");
    print_tree(kn->middle, depth+1);
    if (!is_two_node(kn)) {
        print_depth(depth);
        send_string("b: ");
        print_int(kn->b);
        send_string(": ");
        for (struct Processor* cur = kn->b_processor; cur; cur = cur->next) {
            if (cur->name) {
                send_string(cur->name);
            } else {
                send_string("no_name");
            }
            send_string(", ");
        }
        send_string("\n");
        print_tree(kn->right, depth+1);
    }
}
#endif

uint16_t bubble_a = 0;
struct Processor* bubble_a_processor = NULL;
struct KeyNode* bubble_left = NULL;
struct KeyNode* bubble_middle = NULL;

void reset_bubble(uint16_t a, struct Processor* p) {
    bubble_a = a,
    bubble_a_processor = p;

    bubble_left = NULL;
    bubble_middle = NULL;
}

struct Processor* get_processor(struct KeyNode* kn, uint16_t idx) {
    // Null check
    if (!kn) {
        return NULL;
    }

    // Check if we're at the relevant node
    if (kn->a == idx) {
        return kn->a_processor;
    } else if (is_three_node(kn) && idx == kn->b) {
        return kn->b_processor;
    }

    if (idx < kn->a) {
        return get_processor(kn->left, idx);
    } else if (is_three_node(kn) && idx > kn->b) {
        return get_processor(kn->right, idx);
    }
    return get_processor(kn->middle, idx);
}

// Bubble will always be a two-node
bool insert_bubble(struct KeyNode* kn) {
    if (is_two_node(kn)) {
        if (bubble_a < kn->a) {
            kn->right = kn->middle;
            kn->middle = bubble_middle;
            kn->left = bubble_left;

            kn->b = kn->a;
            kn->b_processor = kn->a_processor;
            kn->a = bubble_a;
            kn->a_processor = bubble_a_processor;
        } else if (bubble_a > kn->a) {
            kn->right = bubble_middle;
            kn->middle = bubble_left;

            kn->b = bubble_a;
            kn->b_processor = bubble_a_processor;
        }
        return false;
    }

    if (bubble_a < kn->a) {
        struct KeyNode* new_n = NewKeyNode(bubble_a, bubble_a_processor);
        if (!new_n) {
            return false;
        }
        new_n->left = bubble_left;
        new_n->middle = bubble_middle;

        bubble_a = kn->a;
        bubble_a_processor = kn->a_processor;
        bubble_left = new_n;
        bubble_middle = kn;

        kn->a = kn->b;
        kn->a_processor = kn->b_processor;
        kn->left = kn->middle;
        kn->middle = kn->right;
        to_two_node(kn);
    } else if (bubble_a > kn->b) {
        struct KeyNode* new_n = NewKeyNode(bubble_a, bubble_a_processor);
        if (!new_n) {
            return false;
        }
        new_n->left = bubble_left;
        new_n->middle = bubble_middle;

        bubble_a = kn->b;
        bubble_a_processor = kn->b_processor;
        bubble_left = kn;
        bubble_middle = new_n;

        to_two_node(kn);
    } else {
        struct KeyNode* new_n = NewKeyNode(kn->b, kn->b_processor);
        if (!new_n) {
            return false;
        }
        new_n->left = bubble_middle;
        new_n->middle = kn->right;

        to_two_node(kn);
        kn->middle = bubble_left;

        bubble_left = kn;
        bubble_middle = new_n;
    }

    return true;
}

bool insert_processor(struct KeyNode* kn, uint16_t idx, struct Processor* p) {
    if (idx == kn->a) {
        append_processor(kn->a_processor, p);
        return false;
    } else if (is_three_node(kn) && idx == kn->b) {
        append_processor(kn->b_processor, p);
        return false;
    }

    // If leaf node (since left is always set), then just insert the bubble here
    if (!kn->left) {
        reset_bubble(idx, p);
        return insert_bubble(kn);
    }

    bool need_to_bubble = false;
    if (idx < kn->a) {
        need_to_bubble = insert_processor(kn->left, idx, p);
    } else if (is_two_node(kn) || idx < kn->b) {
        need_to_bubble = insert_processor(kn->middle, idx, p);
    } else {
        need_to_bubble = insert_processor(kn->right, idx, p);
    }

    return need_to_bubble && insert_bubble(kn);
}

struct KeyNode* InsertProcessor(struct KeyNode* root, uint16_t idx, struct Processor* p) {
    if (!p) {
        return root;
    }
    if (oom_error) {
        return root;
    }
    #if LEEP_DEBUG > 1
        send_string("INSERTING [");
        print_int(idx);
        send_string(", ");
        if (p->name) {
            send_string(p->name);
        }
        send_string("] =======================\n");
    #endif
    if (!root) {
        return NewKeyNode(idx, p);
    }

    if (!insert_processor(root, idx, p)) {
        #if LEEP_DEBUG > 2
            print_tree(root, 0);
        #endif
        return root;
    }

    // If here, then we need to increase the height of the tree
    struct KeyNode* new_root = NewKeyNode(bubble_a, bubble_a_processor);
    if (!new_root) {
        return root;
    }

    new_root->left = bubble_left;
    new_root->middle = bubble_middle;
    #if LEEP_DEBUG > 2
        print_tree(new_root, 0);
    #endif
    return new_root;
}

/********************************
 * Custom Processor methods *
 ********************************/

struct Processor* _simple_processor(LEBUG_ARG(char* name) bool (*f)(struct Processor*)) {
    struct Processor* pd = NewProcessor();
    if (!pd) {
        return NULL;
    }
    pd->f = f;
    LEBUG_STMT(pd->name = name;)
    return pd;
}

struct Processor* _simple_string_processor(LEBUG_ARG(char* name) bool (*f)(struct Processor*), char* str) {
    struct Processor* pd = NewProcessor();
    if (!pd) {
        return NULL;
    }
    pd->data.send_string_arg = str;
    pd->f = f;
    LEBUG_STMT(pd->name = name;)
    return pd;
}

struct Processor* _simple_bool_processor(LEBUG_ARG(char* name) bool (*f)(struct Processor*), bool b) {
    struct Processor* pd = NewProcessor();
    if (!pd) {
        return NULL;
    }
    pd->data.bool_arg = b;
    pd->f = f;
    LEBUG_STMT(pd->name = name;)
    return pd;
}

static bool _send_string(struct Processor* pd) {
    if (!pd) {
        send_string("KEYBOARD_ERR: null _send_string processor");
    }
    send_string((pd->data.send_string_arg));
    return false;
}

// Use STRING_SENDER macro instead of this function
struct Processor* _string_sender(LEBUG_ARG(char* name) char* str) {
    return _simple_string_processor(LEBUG_ARG(name) _send_string, str);
}

bool _send_to_url(struct Processor* pd) {
    if (!pd) {
        send_string("KEYBOARD_ERR: null _send_to_url processor");
    }
    SEND_STRING(SS_DOWN(X_RCTL) "l");
    URLWait();
    send_string(pd->data.send_string_arg);
    SEND_STRING(SS_UP(X_RCTL));
    return false;
}

// Use URL_SENDER macro instead of this function
struct Processor* _url_sender(LEBUG_ARG(char* name) char* str) {
    return _simple_string_processor(LEBUG_ARG(name) _send_to_url, str);
}

bool _send_to_new_tab(struct Processor* pd) {
    if (!pd) {
        send_string("KEYBOARD_ERR: null _send_to_new_tab processor");
    }
    SEND_STRING(SS_RCTL("t"));
    URLWait();
    send_string(pd->data.send_string_arg);
    return false;
}

// Use NEW_TAB_SENDER macro instead of this function
struct Processor* _new_tab_sender(LEBUG_ARG(char* name) char* str) {
    return _simple_string_processor(LEBUG_ARG(name) _send_to_new_tab, str);
}

/***********************
* Procedural functions *
***********************/

bool run_processor(struct Processor* p, bool if_none) {
    bool no_processors = true;
    bool r = false;
    for (struct Processor* cur = p; cur; cur = cur->next) {
        no_processors = false;
        r = r || (cur->f && cur->f(cur));
    }
    return r || (no_processors && if_none);
}

/************************
* Key code registration *
************************/

struct KeyNode* kc_press_root = NULL;
struct KeyNode* kc_unpress_root = NULL;
struct Processor* kc_global_press_processor = NULL;
struct Processor* kc_global_unpress_processor = NULL;

void RegisterGlobalPressFunc(struct Processor* p) {
    kc_global_press_processor = append_processor(kc_global_press_processor, p);
}

void RegisterGlobalUnpressFunc(struct Processor* p) {
    kc_global_unpress_processor = append_processor(kc_global_unpress_processor, p);
}

void RegisterPressFunc(int kc, struct Processor* p) {
    kc_press_root = InsertProcessor(kc_press_root, kc, p);
}

void RegisterUnpressFunc(int kc, struct Processor* p) {
    kc_unpress_root = InsertProcessor(kc_unpress_root, kc, p);
}

bool did_processor_stuff = false;
void init_stuff(void);

bool RunKC(int keycode, keyrecord_t* record) {
    init_stuff();

    // Get the base keycode of a mod or layer tap key
    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            if (!record->tap.count) {
                // Key is being held
                return true;
            }
            // Key is being pressed, so convert it to its regular keycode.
            keycode = keycode & 0xFF;
    }

    struct Processor* p = NULL;
    struct Processor* gp = NULL;
    if (record->event.pressed) {
        p = get_processor(kc_press_root, keycode);
        gp = kc_global_press_processor;
    } else {
        p = get_processor(kc_unpress_root, keycode);
        gp = kc_global_unpress_processor;
    }

    bool r = run_processor(p, true);
    bool rp = run_processor(gp, false);
    return r || rp;
}

/*********************
* Layer registration *
*********************/

struct KeyNode* layer_on_root = NULL;
struct KeyNode* layer_off_root = NULL;
struct Processor* global_layer_processor = NULL;

bool layers_status[NUM_LAYERS];

void RegisterLayerOnFunc(int layer, struct Processor* p) {
    layer_on_root = InsertProcessor(layer_on_root, layer, p);
}

void RegisterLayerOffFunc(int layer, struct Processor* p) {
    layer_off_root = InsertProcessor(layer_off_root, layer, p);
}

// We only have global layer funcs, not specific to whether or not
// the most recent action was on/off.
void RegisterGlobalLayerFunc(struct Processor* p) {
    global_layer_processor = append_processor(global_layer_processor, p);
}

layer_state_t RunLayers(layer_state_t state) {
    init_stuff();

    // Run processors
    for (int i = 0; i < NUM_LAYERS; i++) {
        bool new_state = layer_state_cmp(state, i);
        if (new_state != layers_status[i]) {
            // Layer was turned on
            if (new_state) {
                run_processor(get_processor(layer_on_root, i), false);
            } else {
                run_processor(get_processor(layer_off_root, i), false);
            }
            layers_status[i] = new_state;
        }
    }
    run_processor(global_layer_processor, false);

    on_layer_change(get_highest_layer(state));

    return state;
}

void LayerInit(void) {
    // Base layer starts as on:
    // https://beta.docs.qmk.fm/using-qmk/guides/keymap#keymap-layer-status-id-keymap-layer-status
    layers_status[0] = true;
    for (int i = 1; i < NUM_LAYERS; i++) {
        layers_status[i] = false;
    }
}

/***********************************
* Below are qmk-specific functions *
************************************/
// This function should be defined in keymap.c
void processor_init(void);

void init_stuff(void) {
    if (did_processor_stuff) {
        return;
    }
    did_processor_stuff = true;

    LayerInit(); // this must run before processor_init, otherwise the colors get overwritten
    processor_init();

    // Error resolution
    if (oom_error) {
        send_string("oom error\n");
    }

    // Debugging prints
    LEBUG_STMT(send_string("START TREE PRINT\n");)
    LEBUG_STMT(send_string("press funcs:\n");)
    LEBUG_STMT(print_tree(LEBUG_ARG(kc_press_root) 0);)
    LEBUG_STMT(send_string("unpress funcs:\n");)
    LEBUG_STMT(print_tree(LEBUG_ARG(kc_unpress_root) 0);)
    LEBUG_STMT(send_string("layer on funcs:\n");)
    LEBUG_STMT(print_tree(LEBUG_ARG(layer_on_root) 0);)
    LEBUG_STMT(send_string("layer off funcs:\n");)
    LEBUG_STMT(print_tree(LEBUG_ARG(layer_off_root) 0);)
    LEBUG_STMT(send_string("END TREE PRINT\n");)
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    return RunKC(keycode, record);
}

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    return RunLayers(state);
}

#endif
