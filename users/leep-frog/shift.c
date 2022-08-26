#ifndef LEEP_SHIFT
#define LEEP_SHIFT

#include "processor.c"

bool shift_toggled;

void ToggleShift(void) {
    if (shift_toggled) {
        // Turn off shift.
        SEND_STRING(SS_UP(X_RSFT));
    } else {
        // Turn on shift.
        SEND_STRING(SS_DOWN(X_RSFT));
    }
    shift_toggled = !shift_toggled;
}

bool UntoggleShift(void) {
    if (shift_toggled) {
      ToggleShift();
    }
    return true;
}

// Runs emacs shift toggle (ctrl-j) if hit once, otherwise activates shift
// toggle layer.
void TDToggleShift(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_RCTL("j"));
    } else {
        ToggleShift();
    }
}

// Runs emacs line kill (ctrl-k) if hit once, otherwise emulates behavior for regular text things.
// TODO: Make generic function for single tap vs double tap.
void TDKillLine(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Deactivate shift layer if necessary.
        if (shift_toggled) {
            ToggleShift();
        }
        SEND_STRING(SS_RCTL("k"));
        return;
    }

    // Copy the rest of the line.
    if (!shift_toggled) {
        ToggleShift();
    }
    SEND_STRING(SS_TAP(X_END));
    ToggleShift();

    // Copy and delete it.
    SEND_STRING(SS_RCTL("c") SS_TAP(X_DELETE));
}

bool _ctrl_g_new(bool activated) {
    if (shift_toggled) {
      ToggleShift();
    } else {
      SEND_STRING(SS_RCTL("g"));
    }
    return true;
}

#endif
