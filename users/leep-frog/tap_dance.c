#pragma once

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

int cur_dance(qk_tap_dance_state_t *state, bool interrupt_matters) {
    if (state->count == 1) {
        if ((interrupt_matters && state->interrupted) || !state->pressed) {
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

#include "record.c"
#include "shift.c"

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
            LEEP_SOLID_COLOR(BLUE, false);
            break;
        case 2:
            layer_on(LR_ONE_HAND_LEFT);
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

void shift_each_unpress(qk_tap_dance_state_t *state, void *user_data) {
    switch (shift_press_count) {
        case 1:
            SEND_STRING(SS_UP(X_RSFT));
            LEEP_LAYER_COLOR(LR_BASE, false);
            break;
        case 2:
            layer_off(LR_ONE_HAND_LEFT);
            break;
        default:
            SEND_STRING(SS_UP(X_ENTER));
            break;
    }
}

void shift_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, false)) {
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
            SymbolLayerOverlap_reset();
            break;
        case 2:
            layer_on(LR_ONE_HAND_RIGHT);
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

void symb_each_unpress(qk_tap_dance_state_t *state, void *user_data) {
    switch (symb_press_count) {
        case 1:
            layer_off(LR_SYMB);
            break;
        case 2:
            layer_off(LR_ONE_HAND_RIGHT);
            break;
        default:
            SEND_STRING(SS_UP(X_SPACE));
            break;
    }
}

void symb_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, false)) {
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

// Ctrl-t tap dance
void td_ctrl_t(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, true)) {
        case SINGLE_TAP:
            SEND_STRING(SS_RCTL("t"));
            break;
        case DOUBLE_TAP:
            SEND_STRING(SS_RCTL(SS_RSFT(SS_TAP(X_T))));
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                SEND_STRING(SS_RCTL("t"));
            }
            break;
    }
}

// char *universal_backspace = SS_RCTL(SS_TAP(X_BSPACE) SS_RALT(SS_TAP(X_H)));
// Removed ctrl+alt+h. That was used for bash backspace, but realized
// that we can use 'bind' or 'bindkey' in bash environments to bind
// ctrl+backspace to "backward-kill-word" (found with `bind -P | grep word`)
char *universal_backspace = SS_RCTL(SS_TAP(X_BSPACE));

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
    switch (cur_dance(state, true)) {
        case SINGLE_HOLD:
            // Select all
            SEND_STRING(SS_RCTL("a"));
            break;
        case DOUBLE_HOLD:
            // Select all and copy
            SEND_STRING(SS_RCTL("ac"));
            SNG_COPY();
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                register_code16(KC_A);
            }
            break;
    }
}

void un_tda(qk_tap_dance_state_t *state, void *user_data) { unregister_code16(KC_A); }

void tdb(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, true)) {
        case SINGLE_HOLD:
            // Bulleted list
            SEND_STRING(SS_RCTL(SS_RSFT("8")));
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                register_code16(KC_B);
            }
            break;
    }
}

void un_tdb(qk_tap_dance_state_t *state, void *user_data) { unregister_code16(KC_B); }

void tdc(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, true)) {
        case SINGLE_HOLD:
            // Copy
            SEND_STRING(SS_RCTL("c"));
            SNG_COPY();
            break;
        case DOUBLE_HOLD:
            URL_COPY();
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                register_code16(KC_C);
            }
            break;
    }
}

void un_tdc(qk_tap_dance_state_t *state, void *user_data) { unregister_code16(KC_C); }

void tdi(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, true)) {
        case SINGLE_HOLD:
            CR_ID();
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                register_code16(KC_I);
            }
            break;
    }
}

void un_tdi(qk_tap_dance_state_t *state, void *user_data) { unregister_code16(KC_I); }

void tdu(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, true)) {
        case SINGLE_HOLD:
            URL_COPY();
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                register_code16(KC_U);
            }
            break;
    }
}

void un_tdu(qk_tap_dance_state_t *state, void *user_data) { unregister_code16(KC_U); }

void paste_or_type(qk_tap_dance_state_t *state, void *user_data, uint16_t keycode) {
    switch (cur_dance(state, true)) {
        case SINGLE_HOLD:
            SEND_STRING(SS_PASTE);
            break;
        case DOUBLE_HOLD:
            URL_PASTE();
            break;
        case TRIPLE_TAP:
            // Paste without formatting
            SEND_STRING(SS_RCTL(SS_RSFT("v")));
            break;
        default:
            for (int i = 0; i < state->count; i++) {
                register_code16(keycode);
            }
            break;
    }
}

void tdv(qk_tap_dance_state_t *state, void *user_data) { paste_or_type(state, user_data, KC_V); }

void un_tdv(qk_tap_dance_state_t *state, void *user_data) { unregister_code16(KC_V); }

void tdy(qk_tap_dance_state_t *state, void *user_data) { paste_or_type(state, user_data, KC_Y); }

void un_tdy(qk_tap_dance_state_t *state, void *user_data) { unregister_code16(KC_Y); }

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
    LEEP_SOLID_COLOR(RED, true);
    switch (cur_dance(state, true)) {
        case SINGLE_TAP:
            SNG_RESET();
            while (is_playing_notes()) {
                wait_ms(75);
            }
            break;
        case SINGLE_HOLD:
            SEND_STRING("moonlander_leep-frog.bin" SS_TAP(X_ENTER));
            break;
    }

    reset_keyboard();
}

void scroll_press_left(qk_tap_dance_state_t *state, void *user_data) {
    layer_on(LR_SCROLL);
    if (state->count == 1) {
        return;
    }
    if (state->count == 2) {
        SEND_STRING(SS_RCTL(SS_RGUI(SS_TAP(X_LEFT))));
    }
    SEND_STRING(SS_RCTL(SS_RGUI(SS_TAP(X_LEFT))));
}

void scroll_press_right(qk_tap_dance_state_t *state, void *user_data) {
    layer_on(LR_SCROLL);
    if (state->count == 1) {
        return;
    }
    if (state->count == 2) {
        SEND_STRING(SS_RCTL(SS_RGUI(SS_TAP(X_RIGHT))));
    }
    SEND_STRING(SS_RCTL(SS_RGUI(SS_TAP(X_RIGHT))));
}

void scroll_unpress(qk_tap_dance_state_t *state, void *user_data) { layer_off(LR_SCROLL); }

void scroll_left_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (cur_dance(state, true) == SINGLE_TAP && !state->interrupted) {
        SEND_STRING(SS_RCTL(SS_RGUI(SS_TAP(X_LEFT))));
    }
}

void scroll_right_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (cur_dance(state, true) == SINGLE_TAP && !state->interrupted) {
        SEND_STRING(SS_RCTL(SS_RGUI(SS_TAP(X_RIGHT))));
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    // Shift toggle
    [TDK_SHIFT_TOGGLE] = ACTION_TAP_DANCE_FN(TDToggleShift),
    // Kill line
    [TDK_KILL_LINE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, NULL, TDKillLine_finished, TDKillLine_reset),
    // Record 1
    [TDK_MACRO_1] = ACTION_TAP_DANCE_FN(recorder_1),
    // Record 2
    [TDK_MACRO_2] = ACTION_TAP_DANCE_FN(recorder_2),
    // Markdown paste
    [TDK_MARKDOWN_PASTE] = ACTION_TAP_DANCE_FN(TDMarkdownPaste),
    // Outlook reload
    [TDK_OUTLOOK_RELOAD] = ACTION_TAP_DANCE_FN(TDOutlookReload),
    // Reset keyboard
    [TDK_RESET] = ACTION_TAP_DANCE_FN(TDReset),
    // 'A' tap dance
    [TDK_A] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, NULL, tda, un_tda),
    // 'B' tap dance
    [TDK_B] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, NULL, tdb, un_tdb),
    // 'C' tap dance
    [TDK_C] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, NULL, tdc, un_tdc),
    // 'I' tap dance
    [TDK_I] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, NULL, tdi, un_tdi),
    // 'U' tap dance
    [TDK_U] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, NULL, tdu, un_tdu),
    // 'V' tap dance
    [TDK_V] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, NULL, tdv, un_tdv),
    // 'Y' tap dance
    [TDK_Y] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, NULL, tdy, un_tdy),
    // Ctrl-t tap dance
    [TDK_CTL_T] = ACTION_TAP_DANCE_FN(td_ctrl_t),
    // One hand copy
    [TDK_OH_COPY] = ACTION_TAP_DANCE_FN(oh_copy),
    // One hand paste
    [TDK_OH_PASTE] = ACTION_TAP_DANCE_FN(oh_paste),
    // Symbol layer
    [TDK_SYMB_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(symb_each_press, symb_each_unpress, symb_finished, symb_reset),
    // Shift layer
    [TDK_SHIFT_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(shift_each_press, shift_each_unpress, shift_finished, shift_reset),
    // Scroll left layer
    [TDK_SCROLL_LEFT] = ACTION_TAP_DANCE_FN_ADVANCED(scroll_press_left, scroll_unpress, scroll_left_finished, NULL),
    // Scroll right layer
    [TDK_SCROLL_RIGHT] = ACTION_TAP_DANCE_FN_ADVANCED(scroll_press_right, scroll_unpress, scroll_right_finished, NULL),
};

#define TGL_SHF TD(TDK_SHIFT_TOGGLE)
#define CK_KILL TD(TDK_KILL_LINE)
#define CK_MCR1 TD(TDK_MACRO_1)
#define CK_MCR2 TD(TDK_MACRO_2)
#define CK_DEL_WORD RCTL(KC_DELETE)
#define CK_MDPS TD(TDK_MARKDOWN_PASTE)
// See here for outlook keyboard shortcuts:
// https://support.microsoft.com/en-us/office/keyboard-shortcuts-for-outlook-3cdeb221-7ae5-4c1d-8c1d-9e63216c1efd#PickTab=Windows
// Outlook reload
#define OL_RLD TD(TDK_OUTLOOK_RELOAD)
// Outlook move left pane
#define OL_LEFT RCTL(SH(TAB))
// Outlook move right pane
#define OL_RGHT KC_F6
#define TD_RST TD(TDK_RESET)
#define TD_A TD(TDK_A)
#define TD_B TD(TDK_B)
#define TD_C TD(TDK_C)
#define TD_I TD(TDK_I)
#define TD_U TD(TDK_U)
#define TD_V TD(TDK_V)
#define TD_Y TD(TDK_Y)
#define TD_CTLT TD(TDK_CTL_T)
#define OH_COPY TD(TDK_OH_COPY)
#define OH_PSTE TD(TDK_OH_PASTE)
#define TO_SCRL TD(TDK_SCROLL_LEFT)
#define TO_SCRR TD(TDK_SCROLL_RIGHT)

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
