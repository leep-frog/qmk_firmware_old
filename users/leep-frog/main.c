#ifndef LEEP_MAIN
#define LEEP_MAIN

#include <stdio.h>
#include "interface.c"
#include "enum.c"
#include "define_keys.c"
#include "color.c"
#include "music.c"
#include "google.c"
#include "workspace.c"
#include "record.c"
#include "url.c"
#include "combo.c"
#include "tap_dance.c"
#include "shift.c"
#include "ctrl.c"

// Note: wally sometimes crashes if the keyboard is plugged into the workstation,
// but behaves fine if the keyboard is plugged directly into the laptop.

#ifdef CONSOLE_ENABLE

#    define LEBUG(s, ...) uprintf(s, ##__VA_ARGS__);

void print_int(int k) {
    char c[10];
    itoa(k, c, 10);
    send_string(c);
}

#    define PRINT_INT(i) print_int(i);

#else

#    define LEBUG(s, ...)
#    define PRINT_INT(i)

#endif

bool _ctrl_w_new(void) {
    if (!shift_toggled) {
        return true;
    }
    // Clear toggle
    ToggleShift();
    // Copy contents
    SEND_STRING(SS_COPY);
    // Delete selected text.
    SEND_STRING(SS_TAP(X_DELETE));
    return false;
}

void _rgb_off(bool pressed) {
    if (pressed) {
        // TODO: Also go into keyboard lock mode?
        LEEP_SOLID_COLOR(OFF);
    }
}

void _alt_t_new(bool pressed) {
    if (!pressed) {
        return;
    }

    if (get_mods() & MOD_MASK_SHIFT) {
        // If holding shift, then actually send alt+shift+t
        // shift is already being held so we don't need to specify that here.
        // In fact, we shouldn't, otherwise we would un-tap the shift key, even
        // though it could still be held for more alt+shift keys.
        SEND_STRING(SS_RALT("t"));
    } else {
        // Otherwise, send new tab command (ctrl+shift+t)
        SEND_STRING(SS_RCTL(SS_RSFT("t")));
    }
}

void _ctrl_click(bool pressed) {
    if (!pressed) {
        return;
    }
    // Used to have the following line
    // #define MS_CTRL RCTL(KC_MS_BTN1)
    // but in my work Windows laptop, the ctrl and click would be too
    // close together and sometimes wouldn't work properly.
    SEND_STRING(SS_DOWN(X_RCTL));
    wait_ms(50);
    SEND_STRING(SS_TAP(X_MS_BTN1));
    SEND_STRING(SS_UP(X_RCTL));
}

void _eye_care(bool pressed) {
    if (pressed) {
        // The color change takes effect after the keycode is processed, so we can't
        // change the color twice in the _eye_care function.
        // Instead we set the first color on key down.
        LEEP_SOLID_COLOR(RED);
        SNG_EYE_START;
    } else {
        wait_ms(20 * 1000);
        LEEP_SOLID_COLOR(GREEN);
        SNG_EYE_END;
    }
}

static uint16_t ck_timer;

void _ck_timer(bool pressed) {
    if (pressed) {
        ck_timer = timer_read();
    } else {
        PRINT_INT(timer_elapsed(ck_timer));
    }
}

static uint16_t alt_timer;
bool            leep_alt_interrupted = false;

void _to_alt_fn(bool pressed) {
    if (pressed) {
        leep_alt_interrupted = false;
        alt_timer            = timer_read();
        layer_on(LR_ALT);
    } else {
        layer_off(LR_ALT);
        if (!leep_alt_interrupted && timer_elapsed(alt_timer) < TAPPING_TERM) {
            layer_on(LR_ONE_HAND);
        }
    }
}

void _safe_layer(bool activated) {
    if (!activated) {
        return;
    }

    if (shift_toggled) {
        ToggleShift();
    }
    clear_mods();
}

#define KEY_PROCESSOR_OFFSET(v) C__OFFSET(CK_ENUM_START, v)

typedef void (*processor_action_t)(bool activated);

#define PRC_ACTION(user_fn) user_fn

#define MAX_STRING_LEN 12

#define C__OFFSET(C__START, v) v - C__START - 1

#define PROCESSOR_VALUE1(start, key, value) [C__OFFSET(start, key)] = value,
#define PROCESSOR_VALUE2(start, key, value, ...) PROCESSOR_VALUE1(start, key, value) PROCESSOR_VALUE1(start, __VA_ARGS__)
#define PROCESSOR_VALUE3(start, key, value, ...) PROCESSOR_VALUE1(start, key, value) PROCESSOR_VALUE2(start, __VA_ARGS__)
#define PROCESSOR_VALUE4(start, key, value, ...) PROCESSOR_VALUE1(start, key, value) PROCESSOR_VALUE3(start, __VA_ARGS__)
#define PROCESSOR_VALUE5(start, key, value, ...) PROCESSOR_VALUE1(start, key, value) PROCESSOR_VALUE4(start, __VA_ARGS__)
#define PROCESSOR_VALUE6(start, key, value, ...) PROCESSOR_VALUE1(start, key, value) PROCESSOR_VALUE5(start, __VA_ARGS__)
#define PROCESSOR_VALUE7(start, key, value, ...) PROCESSOR_VALUE1(start, key, value) PROCESSOR_VALUE6(start, __VA_ARGS__)
#define PROCESSOR_VALUE8(start, key, value, ...) PROCESSOR_VALUE1(start, key, value) PROCESSOR_VALUE7(start, __VA_ARGS__)

#define OPTIONAL_PROCESSOR_MACRO(_type_, sz, num_provided, e_start, prefix, suffix, dflt, ...) const _type_ PROGMEM prefix##_processors[sz] suffix = {[0 ... sz - 1] = dflt, PROCESSOR_VALUE##num_provided(e_start, __VA_ARGS__)};

#define PROCESSOR_MACRO(_type_, num, e_start, prefix, suffix, dflt, ...) OPTIONAL_PROCESSOR_MACRO(_type_, num, num, e_start, prefix, suffix, dflt, __VA_ARGS__)

bool leep_toggling_alt = false;

PROCESSOR_MACRO(char, 4, CS_ENUM_START, cs, [MAX_STRING_LEN + 1], "", TGL_ALT, SS_DOWN(X_RALT) SS_TAP(X_TAB), TGL_SLT, SS_DOWN(X_RALT) SS_RSFT(SS_TAP(X_TAB)),
                // KC_ESC actually sends a "`" (KC_GRAVE) character for some reason.
                // Maybe it's something to do with KC_GESC overlapping or something?
                // Who knows why, but we do need this custom keycode regardless to get around that.
                CK_ESC, SS_TAP(X_ESC), CK_UNBS, SS_RCTL(SS_TAP(X_BSPACE)))

PROCESSOR_MACRO(char, 2, CU_ENUM_START, cu, [MAX_STRING_LEN + 1], "", URL_COPY, "c", URL_ICP, SS_TAP(X_RIGHT) SS_RSFT(SS_TAP(X_LEFT)) "c")

PROCESSOR_MACRO(char, 3, CN_ENUM_START, cn, [MAX_STRING_LEN + 1], "", URL_PST, SS_RSFT(SS_TAP(X_INSERT)) SS_TAP(X_ENTER), CK_CL, "cl/" SS_TAP(X_ENTER), CK_MOMA, "moma " SS_TAP(X_ENTER))

PROCESSOR_MACRO(processor_action_t, 8, CK_ENUM_START, ck, , NULL, CK_CTLG, &_ctrl_g_new, CK_MUT1, &_mute_1, CK_MUT2, &_mute_2, CK_ALTT, &_alt_t_new, MS_CTRL, &_ctrl_click, CK_EYE, &_eye_care, CK_RGBF, &_rgb_off, TO_ALT, &_to_alt_fn)

void deactivate_alt(bool activated) {
    if (!activated) {
        leep_toggling_alt = false;
        SEND_STRING(SS_UP(X_RALT));
    }
}

void ctrl_x_layer(bool activated) {
    if (activated) {
        SEND_STRING(SS_RCTL(SS_TAP(X_X)));
    }
}

void ctrl_alt_layer(bool activated) {
    if (activated) {
        SEND_STRING(SS_DOWN(X_RCTL) SS_DOWN(X_RALT));
    } else {
        SEND_STRING(SS_UP(X_RCTL) SS_UP(X_RALT));
    }
}

#define MAKE_LAYER_PROCESSOR(key, func_name) [key] = PRC_ACTION(func_name)

OPTIONAL_PROCESSOR_MACRO(processor_action_t, NUM_LAYERS, 6, -1, layer, , NULL, LR_CTRL_X, &ctrl_x_layer,
                         // Needed to undo SS_DOWN from TGL_ALT and TGL_SLT.
                         LR_ALT, &deactivate_alt, LR_NAVIGATION, &deactivate_alt, LR_ONE_HAND, &deactivate_alt, LR_CTRL_ALT, &ctrl_alt_layer, LR_SAFE, &_safe_layer)

bool layers_status[NUM_LAYERS] = {
    [0]                    = true,
    [1 ... NUM_LAYERS - 1] = false,
};

#define LEEP_CASE(kc, fn) \
    case (kc):            \
        return fn();

bool played_startup_song = false;

void keyboard_post_init_user(void) {
    if (!played_startup_song) {
        LEEP_COLOR_MODE(GREEN, RGB_MATRIX_RAINDROPS);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (!played_startup_song) {
        if (record->event.pressed) {
            return false;
        }
        switch (keycode) {
            case KC_J:
            case KC_F:
                SNG_STARTUP;
            case KC_K:
            case KC_D:
                played_startup_song = true;
                LEEP_LAYER_COLOR(LR_BASE);
                break;
            default:
                return false;
        }
    }

    if (keycode != TO_ALT) {
        leep_alt_interrupted = true;
    }

    // Return if this is being run on key un-pressed.
    if (!record->event.pressed) {
        // Run unpress events for custom keycodes
        switch (keycode) {
            case LEEP_ENUM_CASE(CK):
                if (ck_processors[LEEP_ENUM_OFFSET(CK, keycode)]) {
                    ck_processors[LEEP_ENUM_OFFSET(CK, keycode)](false);
                }
                return false;
        }

        return true;
    }

    bool alt_key_pressed = (keycode == TGL_ALT) || (keycode == TGL_SLT);
    if (alt_key_pressed) {
        // TODO: move all local variables into a single, static setting object
        // so we don't have to worry about naming issues.
        leep_toggling_alt = true;
    }
    // End alt layer if any key other than alt togglers.
    if (leep_toggling_alt && !alt_key_pressed) {
        leep_toggling_alt = false;
        SEND_STRING(SS_UP(X_RALT));
        return false;
    }

    // Untoggle shift the layer for all non-movement keys
    switch (keycode & QK_BASIC_MAX) {
        case KC_HOME ... KC_UP:
        case KC_BSPACE:
            break;
        default:
            // Don't untoggle for ctrl g since that should *only* deactivate the shift layer
            // (and not send ctrl+g afterwards too)
            if (keycode != CK_CTLG) {
                UntoggleShift();
            }
    }

    // We explicitly want all keycodes to return something to
    // 1) prevent custom keycodes from having logic in this switch and in run_array_processor
    // 2) prevent regular keycode logic from getting to custom keycodes (shouldn't actually be a problem but jic)
    switch (keycode) {
        LEEP_CASE(CTRL_W, _ctrl_w_new)
        case LEEP_ENUM_CASE(CS):
            send_string(cs_processors[LEEP_ENUM_OFFSET(CS, keycode)]);
            return false;
        case LEEP_ENUM_CASE(CU):
            SEND_STRING(SS_DOWN(X_RCTL) "l");
            URLWait();
            send_string(cu_processors[LEEP_ENUM_OFFSET(CU, keycode)]);
            SEND_STRING(SS_UP(X_RCTL));
            return false;
        case LEEP_ENUM_CASE(CN):
            NEW_TAB();
            send_string(cn_processors[LEEP_ENUM_OFFSET(CN, keycode)]);
            return false;
        case LEEP_ENUM_CASE(CK):
            if (ck_processors[LEEP_ENUM_OFFSET(CK, keycode)]) {
                ck_processors[LEEP_ENUM_OFFSET(CK, keycode)](true);
            }
            return false;
    }
    return true;
}

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    // Run processors
    for (int i = 0; i < NUM_LAYERS; i++) {
        bool current_state = layer_state_cmp(state, i);
        if (current_state != layers_status[i]) {
            layers_status[i] = current_state;
            if (layer_processors[i]) {
                layer_processors[i](current_state);
            }
        }
    }

    if (!recording && !shift_toggled && played_startup_song) {
        LEEP_LAYER_COLOR(get_highest_layer(state));
    }

    return state;
}

#endif
