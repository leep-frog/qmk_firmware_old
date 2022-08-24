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
            recording_start(macro_1);
        }
    } else if (recording) {
        action = DYN_REC_STOP;
        kr.event.pressed = true;
        recording = false;
        recording_end(macro_1);
    } else {
      recording_play(macro_1);
    }

    process_dynamic_macro(action, &kr);
}

void recorder_1(qk_tap_dance_state_t *state, void *user_data) {
    recorder_base(state, DYN_MACRO_PLAY1, DYN_REC_START1);
}

void recorder_2(qk_tap_dance_state_t *state, void *user_data) {
    recorder_base(state, DYN_MACRO_PLAY2, DYN_REC_START2);
}

// TODO: use dynamic_macro_led_blink(), see https://github.com/qmk/qmk_firmware/blob/master/docs/feature_dynamic_macros.md
/*void recording_blinker(void) {
    if (!recording) {
        // If blink timer is 0, then we haven't been recording for a while.
        if (!blink_timer) {
            return;
        }
        // Otherwise, we probably just stopped recording so turn off the led.
        recording_end();
        blink_timer = 0;
        return;
    }

    if (!blink_timer) {
        // If blink timer is 0, then we just started recording.
        blink_timer = timer_read();
        recording_start();
        blink_on = true;
        return;
    }

    if (timer_elapsed(blink_timer) < 500) {
        return;
    }

    if (blink_on) {
        record_light_off();
    } else {
        record_light_on();
    }
    blink_on = !blink_on;
    blink_timer = timer_read();
}*/

#endif
