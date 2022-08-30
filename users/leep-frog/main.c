#ifndef LEEP_MAIN
#define LEEP_MAIN

#include <stdio.h>
#include "interface.c"
#include "enum.c"
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

#define LEBUG(s, ...) uprintf ( s, ##__VA_ARGS__ );

void print_int(int k) {
  char c[10];
  itoa(k, c, 10);
  send_string(c);
}

#define PRINT_INT(i) print_int(i);

#else

#define LEBUG(s, ...)
#define PRINT_INT(i)

#endif

// TODO: enums should be CK_*, defined should be DK_*

// TODO: move these to separate files
#define CTRL_W RCTL(KC_W)

bool _ctrl_w_new(void) {
    if (!shift_toggled) {
      return true;
    }
    // Clear toggle
    ToggleShift();
    // Copy contents
    SEND_STRING(SS_RCTL(SS_TAP(X_INSERT)));
    // Delete selected text.
    SEND_STRING(SS_TAP(X_DELETE));
    return false;
}

#define DEFINE_SONG_WITH_TEMPO( var_name, sound, tempo ) \
float var_name ## _song[][2] =  sound;\
uint8_t var_name ## _tempo = tempo;

#define DEFINE_SONG(var_name, sound) \
float var_name ## _song[][2] =  sound;\
uint8_t var_name ## _tempo = TEMPO_DEFAULT;

bool _leep_mute = false;

#define LEEP_PLAY_SONG(sng) if (!_leep_mute) {\
  set_tempo( sng ## _tempo );\
  PLAY_SONG( sng ## _song );\
}

#define LEEP_PLAY_LOOP(sng) if (!_leep_mute) {\
  set_tempo( sng ## _tempo );\
  PLAY_SONG( sng ## _song );\
}

bool _mute_1(bool activated) {
  if (_leep_mute) {
    _leep_mute = false;
    on_unmute_1();
  } else {
    on_mute_1();
    _leep_mute = true;
  }
  return false;
}

bool _mute_2(bool activated) {
  if (_leep_mute) {
    _leep_mute = false;
    on_unmute_2();
  } else {
    on_mute_2();
    _leep_mute = true;
  }
  return false;
}

bool _alt_t_new(bool activated) {
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
    return false;
}

bool _ctrl_click_new(void) {
    // Used to have the following line
    // #define MS_CTRL RCTL(KC_MS_BTN1)
    // but in my work Windows laptop, the ctrl and click would be too
    // close together and sometimes wouldn't work properly.
    SEND_STRING(SS_DOWN(X_RCTL));
    wait_ms(15);
    SEND_STRING(SS_TAP(X_MS_BTN1));
    SEND_STRING(SS_UP(X_RCTL));
    return false;
}

bool _safe_layer(bool activated) {
  if (!activated) {
    return false;
  }

  if (shift_toggled) {
      ToggleShift();
  }
  clear_mods();
  return false;
}

// Custom commands
// Note: we use page down and page up because that works with chrome
// and "tab" is a special character in some terminals and sometimes gets
// modified or ignored when passed to processes inside of the terminal.
#define CK_TABF RCTL(KC_PGDOWN)  // Next tab in chrome
#define CK_TABB LCTL(KC_PGUP)    // Previous tab in chrome
#define CK_WWWF LALT(KC_RIGHT)   // Next page in chrome
#define CK_WWWB LALT(KC_LEFT)    // Previous page in chrome

// Mouse movements
#define MS_LEFT KC_MS_BTN1
#define MS_RGHT KC_MS_BTN2
#define MS_MID KC_MS_BTN3
#define MS_SMID RSFT(KC_MS_BTN3)

#define CK_NEW RCTL(RSFT(KC_N))
// Copy with ctrl+c (instead of ctrl+<insert>) so ctrl+c functionality
// still works (like cancelling in bash terminal).
#define CK_COPY RCTL(KC_C)
#define CK_PSTE RSFT(KC_INSERT)
// These use IFTTT and use custom keyboard shortcuts under the hood.
#define LGHT_ON RCTL(RALT(RSFT(KC_8)))
#define LGHT_OF RCTL(RALT(RSFT(KC_9)))

// Layer toggling
#define TO_SFTY TO(LR_SAFE)
#define TO_OTLK LT(LR_OUTLOOK, KC_SCLN)
// Change KC_DELETE and KC_BSPC to something else
// since we should be using ctrl+h, ctrl+d for those
// and don't need the duplication
#define TO_SHCT LT(LR_SHORTCUTS, KC_NO)
#define TO_SYMB LT(LR_SYMB, KC_ENTER)
#define TO_ALT LT(LR_ALT, KC_TAB)
#define TO_CTL LT(LR_CTRL, KC_SPACE)
// We don't change the keyboard color for shift because there isn't
// a way to only do it on mod and not on tap as well, and seeing the
// color change for every space character was really annoying.
#define TO_SFT RSFT_T(KC_SPACE)
#define TO_NAV LT(LR_NAVIGATION, KC_LGUI)
#define TO_CTAL MO(LR_CTRL_ALT)

// Runs just once when the keyboard initializes.
void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  /*debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  LEBUG("initted")
  //debug_mouse=true;*/
}

#define STRING_FUNC(func_name, str) bool func_name(bool activated) {\
  send_string(str);\
  return false;\
}

#define URL_FUNC(func_name, str) bool func_name(bool activated) {\
  SEND_STRING(SS_DOWN(X_RCTL) "l");\
  URLWait();\
  send_string(str);\
  SEND_STRING(SS_UP(X_RCTL));\
  return false;\
}

#define NEW_TAB_FUNC(func_name, str) bool func_name(bool activated) {\
  SEND_STRING(SS_RCTL("t"));\
  URLWait();\
  send_string(str);\
  return false;\
}

STRING_FUNC(alt_tab, SS_DOWN(X_RALT) SS_TAP(X_TAB))
STRING_FUNC(alt_shft_tab, SS_DOWN(X_RALT) SS_RSFT(SS_TAP(X_TAB)))
STRING_FUNC(end_alt_tab, SS_UP(X_RALT))

URL_FUNC(url_copy, "c")
URL_FUNC(url_id_copy, SS_TAP(X_RIGHT) SS_RSFT(SS_TAP(X_LEFT)) "c")
NEW_TAB_FUNC(url_paste, SS_RSFT(SS_TAP(X_INSERT)) SS_TAP(X_ENTER))
//     // KC_ESC actually sends a "`" (KC_GRAVE) character for some reason.
//     // Maybe it's something to do with KC_GESC overlapping or something?
//     // Who knows why, but we do need this custom keycode regardless to get around that.
STRING_FUNC(escape, SS_TAP(X_ESC))

NEW_TAB_FUNC(cl, "cl/" SS_TAP(X_ENTER))
NEW_TAB_FUNC(moma, "moma " SS_TAP(X_ENTER))

STRING_FUNC(uni_bs, universal_backspace)

#define KEY_PROCESSOR_OFFSET(v) v - CK_ENUM_START - 1

typedef bool (*processor_action_t)(bool activated);

#define PRC_ACTION(user_fn) user_fn

/*
typedef bool (*processor_func_type)(bool activated);

typedef struct {
  processor_func_type fn;
} processor_action_t;

#define PRC_ACTION(user_fn) { .fn = user_fn }
*/

#define MAKE_KEY_PROCESSOR(key, func_name) [ KEY_PROCESSOR_OFFSET ( key ) ] = PRC_ACTION ( func_name )

//processor_action_t key_processors[CK_ENUM_END - CK_ENUM_START - 1] = {
#define NUM_KEY_PROCESSORS KEY_PROCESSOR_OFFSET(CK_ENUM_END)
const processor_action_t PROGMEM key_processors[NUM_KEY_PROCESSORS] = {
  [0 ... NUM_KEY_PROCESSORS - 1] = PRC_ACTION( NULL ),
  // TODO: Make this an array of strings (where applicable) to improve memory usage.
  MAKE_KEY_PROCESSOR(TGL_ALT, alt_tab),
  MAKE_KEY_PROCESSOR(TGL_SLT, alt_shft_tab),
  MAKE_KEY_PROCESSOR(TGL_ELT, end_alt_tab),

  MAKE_KEY_PROCESSOR(URL_COPY, url_copy),
  MAKE_KEY_PROCESSOR(URL_ICP, url_id_copy),
  MAKE_KEY_PROCESSOR(URL_PST, url_paste),
  MAKE_KEY_PROCESSOR(CK_CL, cl),
  MAKE_KEY_PROCESSOR(CK_MOMA, moma),
  MAKE_KEY_PROCESSOR(CK_UNBS, uni_bs),

  MAKE_KEY_PROCESSOR(CK_ESC, escape),
  MAKE_KEY_PROCESSOR(CK_CTLG, _ctrl_g_new),
  MAKE_KEY_PROCESSOR(CK_MUT1, _mute_1),
  MAKE_KEY_PROCESSOR(CK_MUT2, _mute_2),
  MAKE_KEY_PROCESSOR(CK_ALTT, _alt_t_new),
};

bool alt_and_or_nav_layer(bool activated) {
  if (!activated) {
    SEND_STRING(SS_UP(X_RALT));
  }
  return true;
}

bool ctrl_x_layer(bool activated) {
  if (activated) {
    SEND_STRING(SS_RCTL(SS_TAP(X_X)));
  }
  return true;
}

bool ctrl_alt_layer(bool activated) {
  if (activated) {
    SEND_STRING(SS_DOWN(X_RCTL) SS_DOWN(X_RALT));
  } else {
    SEND_STRING(SS_UP(X_RCTL) SS_UP(X_RALT));
  }
  return true;
}

#define MAKE_LAYER_PROCESSOR(key, func_name) [key] = PRC_ACTION(func_name)

const processor_action_t PROGMEM layer_processors[NUM_LAYERS] = {
  [0 ... NUM_LAYERS - 1] = PRC_ACTION( NULL ),
  MAKE_LAYER_PROCESSOR(LR_CTRL_X, ctrl_x_layer),
  // Needed to undo SS_DOWN from TGL_ALT and TGL_SLT.
  MAKE_LAYER_PROCESSOR(LR_ALT, alt_and_or_nav_layer),
  // Needed to undo SS_DOWN from TGL_ALT
  MAKE_LAYER_PROCESSOR(LR_NAVIGATION, alt_and_or_nav_layer),
  MAKE_LAYER_PROCESSOR(LR_CTRL_ALT, ctrl_alt_layer),
  MAKE_LAYER_PROCESSOR(LR_SAFE, _safe_layer),
};

// https://beta.docs.qmk.fm/using-qmk/guides/custom_quantum_functions#matrix_scan_-function-documentation
// This gets run at every matrix scan (many times per second), so be careful
// not to put too many things in here, otherwise you may notice performance issues.
//void matrix_scan_user(void) {
    //recording_blinker();
//}

bool layers_status[NUM_LAYERS] = {
  [0] = true,
  [1 ... NUM_LAYERS - 1] = false,
};

bool run_array_processor(const processor_action_t processors[], size_t sz, size_t idx, bool activated) {
  if (idx >= 0 && idx < sz && processors[idx]) {
    processors[idx](activated);
    return false;
  }
  return true;
}

#define LEEP_CASE(kc, fn) case (kc): return fn();

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  // Return if this is being run on key un-pressed.
  if (!record->event.pressed) {
    return true;
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
    LEEP_CASE(RESET, on_reset)
    LEEP_CASE(CTRL_W, _ctrl_w_new)
    LEEP_CASE(MS_CTRL, _ctrl_click_new)
  }

  // The boolean here could be if the key was pressed or unpressed,
  // but not that's currently used so it'd just be extra logic (here and
  // in all implementing functions).
  return run_array_processor(key_processors, NUM_KEY_PROCESSORS, KEY_PROCESSOR_OFFSET(keycode), true);
}

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
  // Run processors
  for (int i = 0; i < NUM_LAYERS; i++) {
    bool current_state = layer_state_cmp(state, i);
    if (current_state != layers_status[i]) {
      run_array_processor(layer_processors, NUM_LAYERS, i, current_state);
      layers_status[i] = current_state;
    }
  }

  on_layer_change(get_highest_layer(state));

  return state;
}

#endif
