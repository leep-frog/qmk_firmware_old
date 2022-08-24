#ifndef LEEP_MAIN
#define LEEP_MAIN

#include "interface.c"
#include "enum.c"
#include "google.c"
#include "workspace.c"
#include "record.c"
#include "url.c"
#include "combo.c"
#include "tap_dance.c"
#include "processor.c"
#include "shift.c"
#include "ctrl.c"

// Note: wally sometimes crashes if the keyboard is plugged into the workstation,
// but behaves fine if the keyboard is plugged directly into the laptop.

// TODO: move these to separate files
#define CTRL_W RCTL(KC_W)

bool _ctrl_w(struct Processor* pd) {
    if (shift_toggled) {
        // Clear toggle
        ToggleShift();
        // Copy contents
        SEND_STRING(SS_RCTL(SS_TAP(X_INSERT)));
        // Delete selected text.
        SEND_STRING(SS_TAP(X_DELETE));
        return false;
    }
    return true;
}

bool _reset(struct Processor* pd) {
  on_reset();
  reset_keyboard();
  return true;
}

bool is_muted = true;
bool _mute(struct Processor* pd) {
  if (is_muted) {
    music_on();
  } else {
    music_off();
  }
  is_muted = !is_muted;
  return true;
}

bool _alt_t(struct Processor* pd) {
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

bool _ctrl_click(struct Processor* pd) {
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

struct Processor* CtrlClickProcessor(void) {
    return SIMPLE_PROCESSOR("_ctrl_click", _ctrl_click);
}

bool _safe_layer(struct Processor* pd) {
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
#define TO_SFT RSFT_T(KC_SPACE)
#define TO_NAV LT(LR_NAVIGATION, KC_LGUI)

// Runs just once when the keyboard initializes.
void keyboard_post_init_user(void) {
}

void processor_init(void) {
    RegisterPressFunc(VRSN, STRING_SENDER("version", QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION));
    RegisterPressFunc(TGL_ALT, STRING_SENDER("alt_tab", SS_DOWN(X_RALT) SS_TAP(X_TAB)));
    RegisterPressFunc(TGL_SLT, STRING_SENDER("alt_shft_tab", SS_DOWN(X_RALT) SS_RSFT(SS_TAP(X_TAB))));
    RegisterPressFunc(TGL_ELT, STRING_SENDER("end_alt_tab", SS_UP(X_RALT)));
    // Needed to undo SS_DOWN from TGL_ALT and TGL_SLT.
    RegisterLayerOffFunc(LR_ALT, STRING_SENDER("end_alt_layer", SS_UP(X_RALT)));
    // Needed to undo SS_DOWN from TGL_ALT
    RegisterLayerOffFunc(LR_NAVIGATION, STRING_SENDER("end_alt_layer_nav", SS_UP(X_RALT)));

    RegisterPressFunc(URL_ICP, URL_SENDER("url_id_copy", SS_TAP(X_RIGHT) SS_RSFT(SS_TAP(X_LEFT)) "c"));

    RegisterPressFunc(URL_COPY, URL_SENDER("url_copy", "c"));

    // KC_ESC actually sends a "`" (KC_GRAVE) character for some reason.
    // Maybe it's something to do with KC_GESC overlapping or something?
    // Who knows why, but we do need this custom keycode regardless to get around that.
    RegisterPressFunc(CK_ESC, STRING_SENDER("escape", SS_TAP(X_ESC)));

    RegisterPressFunc(URL_PST, NEW_TAB_SENDER("new_tab", SS_RSFT(SS_TAP(X_INSERT)) SS_TAP(X_ENTER)));

    RegisterPressFunc(CK_CL, NEW_TAB_SENDER("cl", "cl/" SS_TAP(X_ENTER)));
    RegisterPressFunc(CK_MOMA, NEW_TAB_SENDER("moma", "moma "));

    RegisterPressFunc(CK_CTLG, CtrlGProcessor());

    RegisterPressFunc(CK_UNBS, STRING_SENDER("universal_backspace", universal_backspace));

    // Shift toggling
    struct Processor* su = ToggleShiftProcessor(false);
    RegisterPressFunc(KC_BSPC, su);
    RegisterPressFunc(CK_COPY, su);
    RegisterPressFunc(KC_DELETE, su);

    RegisterPressFunc(CTRL_W, SIMPLE_PROCESSOR("ctrl+w", _ctrl_w));
    RegisterPressFunc(CK_RSET, SIMPLE_PROCESSOR("reset", _reset));
    //RegisterPressFunc(CK_MUTE, SIMPLE_PROCESSOR("mute", _mute));
    //RegisterPressFunc(CK_ALTT, SIMPLE_PROCESSOR("alt+t", _alt_t));

    //RegisterPressFunc(MS_CTRL, CtrlClickProcessor());

    // When going into Ctrl-x layer, we still want to hit ctrl-x
    RegisterLayerOnFunc(LR_CTRL_X, STRING_SENDER("ctrl-x", SS_RCTL(SS_TAP(X_X))));
    RegisterLayerOnFunc(LR_SAFE, SIMPLE_PROCESSOR("safe_layer", _safe_layer));

    RegisterLayerOnFunc(LR_CTRL_ALT, STRING_SENDER("ca_layer_on", SS_DOWN(X_RCTL) SS_DOWN(X_RALT)));
    RegisterLayerOffFunc(LR_CTRL_ALT, STRING_SENDER("ca_layer_off", SS_UP(X_RCTL) SS_UP(X_RALT)));

    return;
};

// https://beta.docs.qmk.fm/using-qmk/guides/custom_quantum_functions#matrix_scan_-function-documentation
// This gets run at every matrix scan (many times per second), so be careful
// not to put too many things in here, otherwise you may notice performance issues.
//void matrix_scan_user(void) {
    //recording_blinker();
//}

#endif
