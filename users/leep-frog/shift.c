#ifndef LEEP_SHIFT
#define LEEP_SHIFT

bool shift_toggled;

void ToggleShift(void) {
    if (shift_toggled) {
        // Turn off shift.
        SEND_STRING(SS_UP(X_RSFT));
        LEEP_LAYER_COLOR(LR_CTRL);
    } else {
        // Turn on shift.
        LEEP_COLOR_MODE(RED, RGB_MATRIX_RAINBOW_PINWHEELS);
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
void TDKillLine(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Shift layer is already untoggle in main.c so no need to untoggle it here.
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

void _ctrl_g_new(bool pressed) {
    if (pressed) {
        if (shift_toggled) {
            ToggleShift();
        } else {
            SEND_STRING(SS_RCTL("g"));
        }
    }
}

#endif
