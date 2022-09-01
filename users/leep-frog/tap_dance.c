#ifndef LEEP_TAP_DANCES
#define LEEP_TAP_DANCES
#include "record.c"
#include "shift.c"

/* Below was copied from https://github.com/samhocevar-forks/qmk-firmware/blob/master/docs/feature_tap_dance.md */

/* Alt then ctrl */

// char *universal_backspace = SS_RCTL(SS_TAP(X_BSPACE) SS_RALT(SS_TAP(X_H)));
// Removed ctrl+alt+h. That was used for bash backspace, but realized
// that we can use 'bind' or 'bindkey' in bash environments to bind
// ctrl+backspace to "backward-kill-word" (found with `bind -P | grep word`)
char *universal_backspace = SS_RCTL(SS_TAP(X_BSPACE));
/*#define UNI_BACKSPACE RCTL(KC_BSPACE)

bool bspace_layer;
bool bspace_registered;

void backspace_tapped (qk_tap_dance_state_t *state, void *user_data) {
    // If we are on the first tap, then we might be just starting to hold the key.
    if (state->count > 1) {
        tap_code16(UNI_BACKSPACE);
    }
}

void backspace_finished (qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1 && state->pressed) {
        layer_on(LR_CTRL_ALT);
        bspace_layer = true;
    } else {
        // clicks extra one we missed from the first guy
        register_code16(UNI_BACKSPACE);
        bspace_registered = true;
    }
}

void backspace_reset (qk_tap_dance_state_t *state, void *user_data) {
    if (bspace_layer) {
        layer_off(LR_CTRL_ALT);
        bspace_layer = false;
    }
    if (bspace_registered) {
        unregister_code16(UNI_BACKSPACE);
        bspace_registered = false;
    }
}

bool del_layer;
bool del_registered;

#define DEL_WORD RCTL(KC_DELETE)

void delete_tapped (qk_tap_dance_state_t *state, void *user_data) {
    // If we are on the first tap, then we might be just starting to hold the key.
    if (state->count > 1) {
        tap_code16(DEL_WORD);
    }
}

void delete_finished (qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1 && state->pressed) {
        layer_on(LR_CTRL_ALT);
        del_layer = true;
    } else {
        // clicks extra one we missed from the first guy
        register_code16(DEL_WORD);
        del_registered = true;
    }
}

void delete_reset (qk_tap_dance_state_t *state, void *user_data) {
    if (del_layer) {
        layer_off(LR_CTRL_ALT);
        del_layer = false;
    }
    if (del_registered) {
        unregister_code16(DEL_WORD);
        del_registered = false;
    }
}*/

void TDMarkdownPaste(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING("]");
        return;
    }
    // Finish markdown link
    SEND_STRING("](" SS_RSFT(SS_TAP(X_INSERT)) ")");
}

void TDOutlookReload(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Switch panes
        send_string(SS_RCTL(SS_RSFT(SS_TAP(X_TAB))));
        // Reload
        tap_code16(KC_F5);
        // Swtich panes again
        send_string(SS_RCTL(SS_TAP(X_TAB)));
        return;
    }
    // Otherwise, only reload
    tap_code16(KC_F5);
}

void TDReset(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    SNG_RESET;
    while (is_playing_notes()) {
      wait_ms(150);
    }
  } else {
    LEEP_SOLID_COLOR(HSV_RED);
  }
  reset_keyboard();
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TDK_SHIFT_TOGGLE]  = ACTION_TAP_DANCE_FN(TDToggleShift),
    [TDK_KILL_LINE]     = ACTION_TAP_DANCE_FN(TDKillLine),
    [TDK_MACRO_1]       = ACTION_TAP_DANCE_FN(recorder_1),
    [TDK_MACRO_2]       = ACTION_TAP_DANCE_FN(recorder_2),
    [TDK_MARKDOWN_PASTE] = ACTION_TAP_DANCE_FN(TDMarkdownPaste),
    [TDK_OUTLOOK_RELOAD] = ACTION_TAP_DANCE_FN(TDOutlookReload),
    [TDK_RESET] = ACTION_TAP_DANCE_FN(TDReset),
    /*[TDK_BACKSPACE] = ACTION_TAP_DANCE_FN_ADVANCED(backspace_tapped, backspace_finished, backspace_reset),
    [TDK_DELETE] = ACTION_TAP_DANCE_FN_ADVANCED(delete_tapped, delete_finished, delete_reset),*/
};

#define TGL_SHF TD(TDK_SHIFT_TOGGLE)
#define CK_KILL TD(TDK_KILL_LINE)
#define CK_MCR1 TD(TDK_MACRO_1)
#define CK_MCR2 TD(TDK_MACRO_2)
/*#define CK_UNIVERSAL_BACKSPACE TD(TDK_BACKSPACE)
#define CK_DEL_WORD TD(TDK_DELETE)*/
#define CK_DEL_WORD RCTL(KC_DELETE)
#define CK_MDPS TD(TDK_MARKDOWN_PASTE)
#define CK_OLRL TD(TDK_OUTLOOK_RELOAD)
#define TD_RST TD(TDK_RESET)

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case TD_RST:
      case CK_MCR1:
      case CK_MCR2:
        // If not recording give extra time to double tap to start recording.
        if (!recording) {
          return TAPPING_TERM + 200;
        }
    }
    return TAPPING_TERM;
}

#endif
