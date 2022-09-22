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

#define SS_PASTE SS_RSFT(SS_TAP(X_INSERT))
#define SS_COPY SS_RCTL(SS_TAP(X_INSERT))

void TDMarkdownPaste(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING("]");
        return;
    }

    // Finish markdown link
    if (state->count == 2) {
      SEND_STRING("](" SS_PASTE ")");
      return;
    }

    SEND_STRING(">>" SS_PASTE "]]");
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

void tda(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    SEND_STRING("a");
    return;
  }

  // Else select all
  SEND_STRING(SS_RCTL(SS_TAP(X_A)));
}

void tdu(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    SEND_STRING("u");
    return;
  }

  // Else copy the url
  SEND_STRING(SS_DOWN(X_RCTL) "l" );
  URLWait();
  SEND_STRING("c" SS_UP(X_RCTL));
}

void tdv(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    SEND_STRING("v");
    return;
  }

  // Else paste
  SEND_STRING(SS_PASTE);
}

void tdy(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    SEND_STRING("y");
    return;
  }

  if (state->count == 2) {
    SEND_STRING(SS_PASTE);
    return;
  }

  // Else URL paste
  // TODO: make this macro
  SEND_STRING(SS_DOWN(X_RCTL) "t" SS_UP(X_RCTL));
  URLWait();
  SEND_STRING(SS_PASTE SS_TAP(X_ENTER));
}

void oh_copy(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    SEND_STRING(SS_COPY);
    return;
  }

  // Else copy the url
  SEND_STRING(SS_DOWN(X_RCTL) "l" );
  URLWait();
  SEND_STRING("c" SS_UP(X_RCTL));
}

void oh_paste(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    SEND_STRING(SS_PASTE);
    return;
  }

  // Else URL paste
  // TODO: make this macro
  SEND_STRING(SS_DOWN(X_RCTL) "t" SS_UP(X_RCTL));
  URLWait();
  SEND_STRING(SS_PASTE SS_TAP(X_ENTER));
}

void TDReset(qk_tap_dance_state_t *state, void *user_data) {
  LEEP_SOLID_COLOR(RED);
  if (state->count == 1) {
    SNG_RESET;
    while (is_playing_notes()) {
      wait_ms(150);
    }
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
    [TDK_A] = ACTION_TAP_DANCE_FN(tda),
    [TDK_U] = ACTION_TAP_DANCE_FN(tdu),
    [TDK_V] = ACTION_TAP_DANCE_FN(tdv),
    [TDK_Y] = ACTION_TAP_DANCE_FN(tdy),
    [TDK_OH_COPY] = ACTION_TAP_DANCE_FN(oh_copy),
    [TDK_OH_PASTE] = ACTION_TAP_DANCE_FN(oh_paste),
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
#define TD_A TD(TDK_A)
#define TD_U TD(TDK_U)
#define TD_V TD(TDK_V)
#define TD_Y TD(TDK_Y)
#define OH_COPY TD(TDK_OH_COPY)
#define OH_PSTE TD(TDK_OH_PASTE)

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
