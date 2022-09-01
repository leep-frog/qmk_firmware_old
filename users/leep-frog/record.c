#ifndef LEEP_RECORD
#define LEEP_RECORD

uint16_t blink_timer;
bool blink_on = false;

// Press twice, record
// Press once, stop if recording, otherwise play
bool recording = false;

void recorder_base(qk_tap_dance_state_t *state, uint16_t play_action, uint16_t start_action) {
    // See below link for details on implementation:
    // https://www.reddit.com/r/olkb/comments/co39nc/help_cant_get_dynamic_macros_working_with_tap/
    keyrecord_t kr;
    kr.event.pressed = false;
    uint16_t action = play_action;
    bool macro_1 = play_action == DYN_MACRO_PLAY1;

    if (state->count > 1) {
        if (!recording) {
            action = start_action;
            recording = true;
            SNG_REC_START;
            LEEP_SYS_COLOR(RGB_RED);
            rgb_matrix_mode(RGB_MATRIX_BREATHING);
        }
    } else if (recording) {
        action = DYN_REC_STOP;
        kr.event.pressed = true;
        recording = false;
        if (macro_1) {
          SNG_REC_1_END;
        } else {
          SNG_REC_2_END;
        }
    } else {
      if (macro_1) {
        SNG_REC_1_PLAY;
      } else {
        SNG_REC_2_PLAY;
      }
    }

    process_dynamic_macro(action, &kr);
}

void recorder_1(qk_tap_dance_state_t *state, void *user_data) {
    recorder_base(state, DYN_MACRO_PLAY1, DYN_REC_START1);
}

void recorder_2(qk_tap_dance_state_t *state, void *user_data) {
    recorder_base(state, DYN_MACRO_PLAY2, DYN_REC_START2);
}

#endif
