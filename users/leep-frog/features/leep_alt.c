
static bool leep_toggling_alt = false;

#define ALT_DOWN() SEND_STRING(SS_DOWN(X_RALT))
#define ALT_UP() SEND_STRING(SS_UP(X_RALT))

bool AltTab_handled(uint16_t keycode, keyrecord_t* record) {
    if (!record->event.pressed) {
        return false;
    }

    bool td_alt_key_pressed = (keycode == CK_ATB) || (keycode == CK_SATB);
    // End alt layer if any key other than alt togglers.
    if (leep_toggling_alt && !td_alt_key_pressed) {
        leep_toggling_alt = false;
        ALT_UP();
        return true;
    }
    return false;
}

void AltTab_deactivate(bool activated) {
    if (!activated) {
        leep_toggling_alt = false;
        ALT_UP();
    }
}

void AltTab_run(bool pressed) {
    if (pressed) {
        leep_toggling_alt = true;
        ALT_DOWN();
        SEND_STRING(SS_TAP(X_TAB));
    }
}

void AltTab_runShift(bool pressed) {
    if (pressed) {
        leep_toggling_alt = true;
        ALT_DOWN();
        SEND_STRING(SS_RSFT(SS_TAP(X_TAB)));
    }
}

#undef START_ALT
#undef END_ALT

/* Old logic (moved from tap_dance)

// TODO: do this with macros?
// ALT_TAB TAP DANCE
int alt_tab_press_count = 0;
int alt_tabs_sent       = 0;

void alt_tab_each_press(qk_tap_dance_state_t *state, void *user_data) {
    alt_tab_press_count++;
    if (leep_toggling_alt) {
        alt_tabs_sent++;
        SEND_STRING(SS_TAP(X_TAB));
        return;
    }

    if (alt_tab_press_count == 1) {
        return;
    }

    leep_toggling_alt = true;
    SEND_STRING(SS_DOWN(X_RALT));
    if (alt_tab_press_count == 2) {
        // Press an extra one for the first press.
        alt_tabs_sent++;
        SEND_STRING(SS_TAP(X_TAB));
    }
    alt_tabs_sent++;
    SEND_STRING(SS_TAP(X_TAB));
}

void alt_tab_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, false)) {
        case SINGLE_TAP:
            if (!leep_toggling_alt && alt_tabs_sent == 0) {
                SEND_STRING(SS_RALT(SS_TAP(X_TAB)));
            }
            break;
        case SINGLE_HOLD:
            if (!leep_toggling_alt) {
                leep_toggling_alt = true;
                SEND_STRING(SS_DOWN(X_RALT));
                if (alt_tabs_sent == 0) {
                    SEND_STRING(SS_TAP(X_TAB));
                }
            }
            break;
    }
}

void alt_tab_reset(qk_tap_dance_state_t *state, void *user_data) {
    alt_tab_press_count = 0;
    alt_tabs_sent       = 0;
}

// SALT_TAB TAP DANCE
int salt_tab_press_count = 0;
int salt_tabs_sent       = 0;

void salt_tab_each_press(qk_tap_dance_state_t *state, void *user_data) {
    salt_tab_press_count++;
    if (leep_toggling_alt) {
        salt_tabs_sent++;
        SEND_STRING(SS_RSFT(SS_TAP(X_TAB)));
        return;
    }

    if (salt_tab_press_count == 1) {
        return;
    }

    leep_toggling_alt = true;
    SEND_STRING(SS_DOWN(X_RALT));
    if (salt_tab_press_count == 2) {
        // Press an extra one for the first press.
        salt_tabs_sent++;
        SEND_STRING(SS_RSFT(SS_TAP(X_TAB)));
    }
    salt_tabs_sent++;
    SEND_STRING(SS_RSFT(SS_TAP(X_TAB)));
}

void salt_tab_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (cur_dance(state, false)) {
        case SINGLE_TAP:
            if (!leep_toggling_alt && salt_tabs_sent == 0) {
                SEND_STRING(SS_RALT(SS_RSFT(SS_TAP(X_TAB))));
            }
            break;
        case SINGLE_HOLD:
            if (!leep_toggling_alt) {
                leep_toggling_alt = true;
                SEND_STRING(SS_DOWN(X_RALT));
                if (salt_tabs_sent == 0) {
                    SEND_STRING(SS_RSFT(SS_TAP(X_TAB)));
                }
            }
            break;
    }
}

void salt_tab_reset(qk_tap_dance_state_t *state, void *user_data) {
    salt_tab_press_count = 0;
    salt_tabs_sent       = 0;
}
*/
