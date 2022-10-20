#ifndef LEEP_TAP_DANCES
#define LEEP_TAP_DANCES
#include "record.c"
#include "shift.c"

// See below link for following functions
// https://github.com/samhocevar-forks/qmk-firmware/blob/master/docs/feature_tap_dance.md#setup
enum {
    SINGLE_TAP        = 1,
    SINGLE_HOLD       = 2,
    DOUBLE_TAP        = 3,
    DOUBLE_HOLD       = 4,
    DOUBLE_SINGLE_TAP = 5,  // send two single taps
    TRIPLE_TAP        = 6,
    TRIPLE_HOLD       = 7
};

int cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        // NOTE: This line has been modified from the copied version (only check state->pressed, not state->interrupted)
        //       Specifically for the shift and alt layers
        if (!state->pressed) {
            return SINGLE_TAP;
            // key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
        } else {
            return SINGLE_HOLD;
        }
    } else if (state->count == 2) {
        // DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.

        // NOTE: These lines have also been changed (check state->pressed before checking state->interrupted)
        if (state->pressed) {
            return DOUBLE_HOLD;
        } else if (state->interrupted) {
            return DOUBLE_SINGLE_TAP;
        } else {
            return DOUBLE_TAP;
        }
    }
    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) {
            return TRIPLE_TAP;
        } else {
            return TRIPLE_HOLD;
        }
    } else {
        return 8;  // magic number. At some point this method will expand to work for more presses
    }
}
// End copy

// The shift and symbol tap dances change layers which requires special logic.
// If we just use the logic above (SINGLE_TAP, DOUBLE_TAP, etc. cases), then we
// have the following issue:
// - Do tap dance (DOUBLE_TAP_HOLD let's say) to change layer
// - press another key before tapping_term is up
// - the key is pressed in the original layer, not in the layer we are
//   trying to change to.

// SHIFT TAP DANCE
int shift_press_count = 0;

void shift_each_press(qk_tap_dance_state_t *state, void *user_data) {
    switch (++shift_press_count) {
        case 1:
            SEND_STRING(SS_DOWN(X_RSFT));
            LEEP_SOLID_COLOR(BLUE);
            break;
        case 2:
            layer_on(LR_ONE_HAND);
            break;
        case 3:
            SEND_STRING(SS_TAP(X_ENTER) SS_TAP(X_ENTER));
            // Notice no 'break' here, so we just press a third enter
            // by falling to the default case.
        default:
            // Undo previous press and then press again.
            SEND_STRING(SS_DOWN(X_ENTER));
            break;
    }
}

void shift_each_unpress(void) {
    switch (shift_press_count) {
        case 1:
            SEND_STRING(SS_UP(X_RSFT));
            LEEP_LAYER_COLOR(LR_BASE);
            break;
        case 2:
            layer_off(LR_ONE_HAND);
            break;
        default:
            SEND_STRING(SS_UP(X_ENTER));
            break;
    }
}

void shift_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state)) {
        case SINGLE_TAP:
            SEND_STRING(SS_TAP(X_ENTER));
            break;
        case DOUBLE_TAP:
        case DOUBLE_SINGLE_TAP:
            SEND_STRING(SS_TAP(X_ENTER) SS_TAP(X_ENTER));
            break;
    }
}

void shift_reset(qk_tap_dance_state_t *state, void *user_data) { shift_press_count = 0; }

// SYMB TAP DANCE
int symb_press_count = 0;

void symb_each_press(qk_tap_dance_state_t *state, void *user_data) {
    switch (++symb_press_count) {
        case 1:
            layer_on(LR_SYMB);
            break;
        case 2:
            layer_on(LR_ONE_HAND);
            break;
        case 3:
            SEND_STRING(SS_TAP(X_SPACE) SS_TAP(X_SPACE));
            // Notice no 'break' here, so we just press a third space
            // by falling to the default case.
        default:
            SEND_STRING(SS_DOWN(X_SPACE));
            break;
    }
}

void symb_each_unpress(void) {
    switch (symb_press_count) {
        case 1:
            layer_off(LR_SYMB);
            break;
        case 2:
            layer_off(LR_ONE_HAND);
            break;
        default:
            SEND_STRING(SS_UP(X_SPACE));
            break;
    }
}

void symb_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state)) {
        case SINGLE_TAP:
            SEND_STRING(SS_TAP(X_SPACE));
            break;
        case DOUBLE_TAP:
        case DOUBLE_SINGLE_TAP:
            SEND_STRING(SS_TAP(X_SPACE) SS_TAP(X_SPACE));
            break;
    }
}

void symb_reset(qk_tap_dance_state_t *state, void *user_data) { symb_press_count = 0; }

// char *universal_backspace = SS_RCTL(SS_TAP(X_BSPACE) SS_RALT(SS_TAP(X_H)));
// Removed ctrl+alt+h. That was used for bash backspace, but realized
// that we can use 'bind' or 'bindkey' in bash environments to bind
// ctrl+backspace to "backward-kill-word" (found with `bind -P | grep word`)
char *universal_backspace = SS_RCTL(SS_TAP(X_BSPACE));

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
    switch (cur_dance(state)) {
        case DOUBLE_TAP:
            // Select all and copy
            SEND_STRING(SS_RCTL("ac"));
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                tap_code16(KC_A);
            }
            break;
    }
}

void tdc(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state)) {
        case DOUBLE_TAP:
            // Copy
            SEND_STRING(SS_RCTL("c"));
            break;
        case TRIPLE_TAP:
            URL_COPY();
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                tap_code16(KC_C);
            }
            break;
    }
}

void tdv(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state)) {
        case DOUBLE_TAP:
            // Paste
            SEND_STRING(SS_RCTL("v"));
            break;
        case TRIPLE_TAP:
            URL_PASTE();
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                tap_code16(KC_V);
            }
            break;
    }
}

void tdu(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING("u");
        return;
    }

    // Else copy the url
    URL_COPY();
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

    URL_PASTE();
}

void oh_copy(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_COPY);
        return;
    }

    URL_COPY();
}

void oh_paste(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_PASTE);
        return;
    }

    URL_PASTE();
}

void TDReset(qk_tap_dance_state_t *state, void *user_data) {
    LEEP_SOLID_COLOR(RED);
    if (cur_dance(state) == SINGLE_TAP) {
        SNG_RESET;
        while (is_playing_notes()) {
            wait_ms(75);
        }
    }
    reset_keyboard();
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TDK_SYMB_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(symb_each_press, symb_finished, symb_reset), [TDK_SHIFT_TOGGLE] = ACTION_TAP_DANCE_FN(TDToggleShift), [TDK_KILL_LINE] = ACTION_TAP_DANCE_FN(TDKillLine), [TDK_MACRO_1] = ACTION_TAP_DANCE_FN(recorder_1), [TDK_MACRO_2] = ACTION_TAP_DANCE_FN(recorder_2), [TDK_MARKDOWN_PASTE] = ACTION_TAP_DANCE_FN(TDMarkdownPaste), [TDK_OUTLOOK_RELOAD] = ACTION_TAP_DANCE_FN(TDOutlookReload), [TDK_RESET] = ACTION_TAP_DANCE_FN(TDReset), [TDK_A] = ACTION_TAP_DANCE_FN(tda), [TDK_C] = ACTION_TAP_DANCE_FN(tdc), [TDK_U] = ACTION_TAP_DANCE_FN(tdu), [TDK_V] = ACTION_TAP_DANCE_FN(tdv), [TDK_Y] = ACTION_TAP_DANCE_FN(tdy), [TDK_OH_COPY] = ACTION_TAP_DANCE_FN(oh_copy), [TDK_OH_PASTE] = ACTION_TAP_DANCE_FN(oh_paste), [TDK_SHIFT_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(shift_each_press, shift_finished, shift_reset),
};

#define TGL_SHF TD(TDK_SHIFT_TOGGLE)
#define CK_KILL TD(TDK_KILL_LINE)
#define CK_MCR1 TD(TDK_MACRO_1)
#define CK_MCR2 TD(TDK_MACRO_2)
#define CK_DEL_WORD RCTL(KC_DELETE)
#define CK_MDPS TD(TDK_MARKDOWN_PASTE)
#define CK_OLRL TD(TDK_OUTLOOK_RELOAD)
#define TD_RST TD(TDK_RESET)
#define TD_A TD(TDK_A)
#define TD_C TD(TDK_C)
#define TD_U TD(TDK_U)
#define TD_V TD(TDK_V)
#define TD_Y TD(TDK_Y)
#define OH_COPY TD(TDK_OH_COPY)
#define OH_PSTE TD(TDK_OH_PASTE)

#define TO_SFT TD(TDK_SHIFT_LAYER)
#define TO_SYMB TD(TDK_SYMB_LAYER)

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
