/* Copyright 2016-2017 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include "quantum.h"

#ifdef BLUETOOTH_ENABLE
#    include "outputselect.h"
#endif

#ifdef BACKLIGHT_ENABLE
#    include "backlight.h"
#endif

#ifdef FAUXCLICKY_ENABLE
#    include "fauxclicky.h"
#endif

#ifdef API_ENABLE
#    include "api.h"
#endif

#ifdef MIDI_ENABLE
#    include "process_midi.h"
#endif

#ifdef VELOCIKEY_ENABLE
#    include "velocikey.h"
#endif

#ifdef HAPTIC_ENABLE
#    include "haptic.h"
#endif

#ifdef AUDIO_ENABLE
#    ifndef GOODBYE_SONG
#        define GOODBYE_SONG SONG(GOODBYE_SOUND)
#    endif
float goodbye_song[][2] = GOODBYE_SONG;
#    ifdef DEFAULT_LAYER_SONGS
float default_layer_songs[][16][2] = DEFAULT_LAYER_SONGS;
#    endif
#endif

#ifdef AUTO_SHIFT_ENABLE
#    include "process_auto_shift.h"
#endif

static void do_code16(uint16_t code, void (*f)(uint8_t)) {
    switch (code) {
        case QK_MODS ... QK_MODS_MAX:
            break;
        default:
            return;
    }

    uint8_t mods_to_send = 0;

    if (code & QK_RMODS_MIN) {  // Right mod flag is set
        if (code & QK_LCTL) mods_to_send |= MOD_BIT(KC_RCTL);
        if (code & QK_LSFT) mods_to_send |= MOD_BIT(KC_RSFT);
        if (code & QK_LALT) mods_to_send |= MOD_BIT(KC_RALT);
        if (code & QK_LGUI) mods_to_send |= MOD_BIT(KC_RGUI);
    } else {
        if (code & QK_LCTL) mods_to_send |= MOD_BIT(KC_LCTL);
        if (code & QK_LSFT) mods_to_send |= MOD_BIT(KC_LSFT);
        if (code & QK_LALT) mods_to_send |= MOD_BIT(KC_LALT);
        if (code & QK_LGUI) mods_to_send |= MOD_BIT(KC_LGUI);
    }

    f(mods_to_send);
}

void register_code16(uint16_t code) {
    if (IS_MOD(code) || code == KC_NO) {
        do_code16(code, register_mods);
    } else {
        do_code16(code, register_weak_mods);
    }
    register_code(code);
}

void unregister_code16(uint16_t code) {
    unregister_code(code);
    if (IS_MOD(code) || code == KC_NO) {
        do_code16(code, unregister_mods);
    } else {
        do_code16(code, unregister_weak_mods);
    }
}

void tap_code16(uint16_t code) {
    register_code16(code);
#if TAP_CODE_DELAY > 0
    wait_ms(TAP_CODE_DELAY);
#endif
    unregister_code16(code);
}

__attribute__((weak)) bool process_action_kb(keyrecord_t *record) { return true; }

__attribute__((weak)) bool process_record_kb(uint16_t keycode, keyrecord_t *record) { return process_record_user(keycode, record); }

__attribute__((weak)) bool process_record_user(uint16_t keycode, keyrecord_t *record) { return true; }

__attribute__((weak)) void post_process_record_kb(uint16_t keycode, keyrecord_t *record) { post_process_record_user(keycode, record); }

__attribute__((weak)) void post_process_record_user(uint16_t keycode, keyrecord_t *record) {}

void reset_keyboard(void) {
    clear_keyboard();
#if defined(MIDI_ENABLE) && defined(MIDI_BASIC)
    process_midi_all_notes_off();
#endif
#ifdef AUDIO_ENABLE
#    ifndef NO_MUSIC_MODE
    music_all_notes_off();
#    endif
    uint16_t timer_start = timer_read();
    PLAY_SONG(goodbye_song);
    shutdown_user();
    while (timer_elapsed(timer_start) < 250) wait_ms(1);
    stop_all_notes();
#else
    shutdown_user();
    wait_ms(250);
#endif
#ifdef HAPTIC_ENABLE
    haptic_shutdown();
#endif
    bootloader_jump();
}

/* Convert record into usable keycode via the contained event. */
uint16_t get_record_keycode(keyrecord_t *record, bool update_layer_cache) {
#ifdef COMBO_ENABLE
    if (record->keycode) {
        return record->keycode;
    }
#endif
    return get_event_keycode(record->event, update_layer_cache);
}

/* Convert event into usable keycode. Checks the layer cache to ensure that it
 * retains the correct keycode after a layer change, if the key is still pressed.
 * "update_layer_cache" is to ensure that it only updates the layer cache when
 * appropriate, otherwise, it will update it and cause layer tap (and other keys)
 * from triggering properly.
 */
uint16_t get_event_keycode(keyevent_t event, bool update_layer_cache) {
#if !defined(NO_ACTION_LAYER) && !defined(STRICT_LAYER_RELEASE)
    /* TODO: Use store_or_get_action() or a similar function. */
    if (!disable_action_cache) {
        uint8_t layer;

        if (event.pressed && update_layer_cache) {
            layer = layer_switch_get_layer(event.key);
            update_source_layers_cache(event.key, layer);
        } else {
            layer = read_source_layers_cache(event.key);
        }
        return keymap_key_to_keycode(layer, event.key);
    } else
#endif
        return keymap_key_to_keycode(layer_switch_get_layer(event.key), event.key);
}

/* Get keycode, and then process pre tapping functionality */
bool pre_process_record_quantum(keyrecord_t *record) {
    if (!(
#ifdef COMBO_ENABLE
            process_combo(get_record_keycode(record, true), record) &&
#endif
            true)) {
        return false;
    }
    return true;  // continue processing
}

/* Get keycode, and then call keyboard function */
void post_process_record_quantum(keyrecord_t *record) {
    uint16_t keycode = get_record_keycode(record, false);
    post_process_record_kb(keycode, record);
}

/* Core keycode function, hands off handling to other functions,
    then processes internal quantum keycodes, and then processes
    ACTIONs.                                                      */
bool process_record_quantum(keyrecord_t *record) {
    uint16_t keycode = get_record_keycode(record, true);

    // This is how you use actions here
    // if (keycode == KC_LEAD) {
    //   action_t action;
    //   action.code = ACTION_DEFAULT_LAYER_SET(0);
    //   process_action(record, action);
    //   return false;
    // }

#ifdef VELOCIKEY_ENABLE
    if (velocikey_enabled() && record->event.pressed) {
        velocikey_accelerate();
    }
#endif

#ifdef WPM_ENABLE
    if (record->event.pressed) {
        update_wpm(keycode);
    }
#endif

#ifdef TAP_DANCE_ENABLE
    preprocess_tap_dance(keycode, record);
#endif

    if (!(
#if defined(KEY_LOCK_ENABLE)
            // Must run first to be able to mask key_up events.
            process_key_lock(&keycode, record) &&
#endif
#if defined(DYNAMIC_MACRO_ENABLE) && !defined(DYNAMIC_MACRO_USER_CALL)
            // Must run asap to ensure all keypresses are recorded.
            process_dynamic_macro(keycode, record) &&
#endif
#if defined(AUDIO_ENABLE) && defined(AUDIO_CLICKY)
            process_clicky(keycode, record) &&
#endif  // AUDIO_CLICKY
#ifdef HAPTIC_ENABLE
            process_haptic(keycode, record) &&
#endif  // HAPTIC_ENABLE
#if defined(RGB_MATRIX_ENABLE)
            process_rgb_matrix(keycode, record) &&
#endif
#ifdef ORYX_ENABLE
            process_record_oryx(keycode, record) &&
#endif
#if defined(VIA_ENABLE)
            process_record_via(keycode, record) &&
#endif
            process_record_kb(keycode, record) &&
#if defined(SEQUENCER_ENABLE)
            process_sequencer(keycode, record) &&
#endif
#if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
            process_midi(keycode, record) &&
#endif
#ifdef AUDIO_ENABLE
            process_audio(keycode, record) &&
#endif
#ifdef BACKLIGHT_ENABLE
            process_backlight(keycode, record) &&
#endif
#ifdef STENO_ENABLE
            process_steno(keycode, record) &&
#endif
#if (defined(AUDIO_ENABLE) || (defined(MIDI_ENABLE) && defined(MIDI_BASIC))) && !defined(NO_MUSIC_MODE)
            process_music(keycode, record) &&
#endif
#ifdef TAP_DANCE_ENABLE
            process_tap_dance(keycode, record) &&
#endif
#if defined(UNICODE_ENABLE) || defined(UNICODEMAP_ENABLE) || defined(UCIS_ENABLE)
            process_unicode_common(keycode, record) &&
#endif
#ifdef LEADER_ENABLE
            process_leader(keycode, record) &&
#endif
#ifdef PRINTING_ENABLE
            process_printer(keycode, record) &&
#endif
#ifdef AUTO_SHIFT_ENABLE
            process_auto_shift(keycode, record) &&
#endif
#ifdef TERMINAL_ENABLE
            process_terminal(keycode, record) &&
#endif
#ifdef SPACE_CADET_ENABLE
            process_space_cadet(keycode, record) &&
#endif
#ifdef MAGIC_KEYCODE_ENABLE
            process_magic(keycode, record) &&
#endif
#ifdef GRAVE_ESC_ENABLE
            process_grave_esc(keycode, record) &&
#endif
#if defined(RGBLIGHT_ENABLE) || defined(RGB_MATRIX_ENABLE)
            process_rgb(keycode, record) &&
#endif
#ifdef JOYSTICK_ENABLE
            process_joystick(keycode, record) &&
#endif
            true)) {
        return false;
    }

    if (record->event.pressed) {
        switch (keycode) {
#ifndef NO_RESET
            case RESET:
                reset_keyboard();
                return false;
#endif
#ifndef NO_DEBUG
            case DEBUG:
                debug_enable ^= 1;
                if (debug_enable) {
                    print("DEBUG: enabled.\n");
                } else {
                    print("DEBUG: disabled.\n");
                }
#endif
                return false;
            case EEPROM_RESET:
                eeconfig_init();
                return false;
#ifdef FAUXCLICKY_ENABLE
            case FC_TOG:
                FAUXCLICKY_TOGGLE;
                return false;
            case FC_ON:
                FAUXCLICKY_ON;
                return false;
            case FC_OFF:
                FAUXCLICKY_OFF;
                return false;
#endif
#ifdef VELOCIKEY_ENABLE
            case VLK_TOG:
                velocikey_toggle();
                return false;
#endif
#ifdef BLUETOOTH_ENABLE
            case OUT_AUTO:
                set_output(OUTPUT_AUTO);
                return false;
            case OUT_USB:
                set_output(OUTPUT_USB);
                return false;
            case OUT_BT:
                set_output(OUTPUT_BLUETOOTH);
                return false;
#endif
#if defined(BACKLIGHT_ENABLE) && defined(BACKLIGHT_BREATHING)
            case BL_BRTG:
                backlight_toggle_breathing();
                return false;
#endif
#ifdef WEBUSB_ENABLE
            case WEBUSB_PAIR:
                webusb_state.pairing ^= true;
                return false;
#endif
        }
    }

    return process_action_kb(record);
}

// clang-format off

/* Bit-Packed look-up table to convert an ASCII character to whether
 * [Shift] needs to be sent with the keycode.
 */
__attribute__((weak)) const uint8_t ascii_to_shift_lut[16] PROGMEM = {
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),

    KCLUT_ENTRY(0, 1, 1, 1, 1, 1, 1, 0),
    KCLUT_ENTRY(1, 1, 1, 1, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 1, 0, 1, 0, 1, 1),
    KCLUT_ENTRY(1, 1, 1, 1, 1, 1, 1, 1),
    KCLUT_ENTRY(1, 1, 1, 1, 1, 1, 1, 1),
    KCLUT_ENTRY(1, 1, 1, 1, 1, 1, 1, 1),
    KCLUT_ENTRY(1, 1, 1, 0, 0, 0, 1, 1),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 1, 1, 1, 1, 0),
};

/* Bit-Packed look-up table to convert an ASCII character to whether
 * [AltGr] needs to be sent with the keycode.
 */
__attribute__((weak)) const uint8_t ascii_to_altgr_lut[16] PROGMEM = {
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),

    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
};

/* Look-up table to convert an ASCII character to a keycode.
 */
__attribute__((weak)) const uint8_t ascii_to_keycode_lut[128] PROGMEM = {
    // NUL   SOH      STX      ETX      EOT      ENQ      ACK      BEL
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // BS    TAB      LF       VT       FF       CR       SO       SI
    KC_BSPC, KC_TAB,  KC_ENT,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // DLE   DC1      DC2      DC3      DC4      NAK      SYN      ETB
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // CAN   EM       SUB      ESC      FS       GS       RS       US
    XXXXXXX, XXXXXXX, XXXXXXX, KC_ESC,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

    //       !        "        #        $        %        &        '
    KC_SPC,  KC_1,    KC_QUOT, KC_3,    KC_4,    KC_5,    KC_7,    KC_QUOT,
    // (     )        *        +        ,        -        .        /
    KC_9,    KC_0,    KC_8,    KC_EQL,  KC_COMM, KC_MINS, KC_DOT,  KC_SLSH,
    // 0     1        2        3        4        5        6        7
    KC_0,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,
    // 8     9        :        ;        <        =        >        ?
    KC_8,    KC_9,    KC_SCLN, KC_SCLN, KC_COMM, KC_EQL,  KC_DOT,  KC_SLSH,
    // @     A        B        C        D        E        F        G
    KC_2,    KC_A,    KC_B,    KC_C,    KC_D,    KC_E,    KC_F,    KC_G,
    // H     I        J        K        L        M        N        O
    KC_H,    KC_I,    KC_J,    KC_K,    KC_L,    KC_M,    KC_N,    KC_O,
    // P     Q        R        S        T        U        V        W
    KC_P,    KC_Q,    KC_R,    KC_S,    KC_T,    KC_U,    KC_V,    KC_W,
    // X     Y        Z        [        \        ]        ^        _
    KC_X,    KC_Y,    KC_Z,    KC_LBRC, KC_BSLS, KC_RBRC, KC_6,    KC_MINS,
    // `     a        b        c        d        e        f        g
    KC_GRV,  KC_A,    KC_B,    KC_C,    KC_D,    KC_E,    KC_F,    KC_G,
    // h     i        j        k        l        m        n        o
    KC_H,    KC_I,    KC_J,    KC_K,    KC_L,    KC_M,    KC_N,    KC_O,
    // p     q        r        s        t        u        v        w
    KC_P,    KC_Q,    KC_R,    KC_S,    KC_T,    KC_U,    KC_V,    KC_W,
    // x     y        z        {        |        }        ~        DEL
    KC_X,    KC_Y,    KC_Z,    KC_LBRC, KC_BSLS, KC_RBRC, KC_GRV,  KC_DEL
};

// clang-format on

// Note: we bit-pack in "reverse" order to optimize loading
#define PGM_LOADBIT(mem, pos) ((pgm_read_byte(&((mem)[(pos) / 8])) >> ((pos) % 8)) & 0x01)

void send_string(const char *str) { send_string_with_delay(str, 0); }

void send_string_P(const char *str) { send_string_with_delay_P(str, 0); }

void send_string_with_delay(const char *str, uint8_t interval) {
    while (1) {
        char ascii_code = *str;
        if (!ascii_code) break;
        if (ascii_code == SS_QMK_PREFIX) {
            ascii_code = *(++str);
            if (ascii_code == SS_TAP_CODE) {
                // tap
                uint8_t keycode = *(++str);
                tap_code(keycode);
            } else if (ascii_code == SS_DOWN_CODE) {
                // down
                uint8_t keycode = *(++str);
                register_code(keycode);
            } else if (ascii_code == SS_UP_CODE) {
                // up
                uint8_t keycode = *(++str);
                unregister_code(keycode);
            } else if (ascii_code == SS_DELAY_CODE) {
                // delay
                int     ms      = 0;
                uint8_t keycode = *(++str);
                while (isdigit(keycode)) {
                    ms *= 10;
                    ms += keycode - '0';
                    keycode = *(++str);
                }
                while (ms--) wait_ms(1);
            }
        } else {
            send_char(ascii_code);
        }
        ++str;
        // interval
        {
            uint8_t ms = interval;
            while (ms--) wait_ms(1);
        }
    }
}

void send_string_with_delay_P(const char *str, uint8_t interval) {
    while (1) {
        char ascii_code = pgm_read_byte(str);
        if (!ascii_code) break;
        if (ascii_code == SS_QMK_PREFIX) {
            ascii_code = pgm_read_byte(++str);
            if (ascii_code == SS_TAP_CODE) {
                // tap
                uint8_t keycode = pgm_read_byte(++str);
                tap_code(keycode);
            } else if (ascii_code == SS_DOWN_CODE) {
                // down
                uint8_t keycode = pgm_read_byte(++str);
                register_code(keycode);
            } else if (ascii_code == SS_UP_CODE) {
                // up
                uint8_t keycode = pgm_read_byte(++str);
                unregister_code(keycode);
            } else if (ascii_code == SS_DELAY_CODE) {
                // delay
                int     ms      = 0;
                uint8_t keycode = pgm_read_byte(++str);
                while (isdigit(keycode)) {
                    ms *= 10;
                    ms += keycode - '0';
                    keycode = pgm_read_byte(++str);
                }
                while (ms--) wait_ms(1);
            }
        } else {
            send_char(ascii_code);
        }
        ++str;
        // interval
        {
            uint8_t ms = interval;
            while (ms--) wait_ms(1);
        }
    }
}

void send_char(char ascii_code) {
#if defined(AUDIO_ENABLE) && defined(SENDSTRING_BELL)
    if (ascii_code == '\a') {  // BEL
        PLAY_SONG(bell_song);
        return;
    }
#endif

    uint8_t keycode    = pgm_read_byte(&ascii_to_keycode_lut[(uint8_t)ascii_code]);
    bool    is_shifted = PGM_LOADBIT(ascii_to_shift_lut, (uint8_t)ascii_code);
    bool    is_altgred = PGM_LOADBIT(ascii_to_altgr_lut, (uint8_t)ascii_code);

    if (is_shifted) {
        register_code(KC_LSFT);
    }
    if (is_altgred) {
        register_code(KC_RALT);
    }
    tap_code(keycode);
    if (is_altgred) {
        unregister_code(KC_RALT);
    }
    if (is_shifted) {
        unregister_code(KC_LSFT);
    }
}

void set_single_persistent_default_layer(uint8_t default_layer) {
#if defined(AUDIO_ENABLE) && defined(DEFAULT_LAYER_SONGS)
    PLAY_SONG(default_layer_songs[default_layer]);
#endif
    eeconfig_update_default_layer(1U << default_layer);
    default_layer_set(1U << default_layer);
}

layer_state_t update_tri_layer_state(layer_state_t state, uint8_t layer1, uint8_t layer2, uint8_t layer3) {
    layer_state_t mask12 = (1UL << layer1) | (1UL << layer2);
    layer_state_t mask3  = 1UL << layer3;
    return (state & mask12) == mask12 ? (state | mask3) : (state & ~mask3);
}

void update_tri_layer(uint8_t layer1, uint8_t layer2, uint8_t layer3) { layer_state_set(update_tri_layer_state(layer_state, layer1, layer2, layer3)); }

void tap_random_base64(void) {
#if defined(__AVR_ATmega32U4__)
    uint8_t key = (TCNT0 + TCNT1 + TCNT3 + TCNT4) % 64;
#else
    uint8_t key = rand() % 64;
#endif
    switch (key) {
        case 0 ... 25:
            send_char(key + 'A');
            break;
        case 26 ... 51:
            send_char(key - 26 + 'a');
            break;
        case 52:
            send_char('0');
            break;
        case 53 ... 61:
            send_char(key - 53 + '1');
            break;
        case 62:
            send_char('+');
            break;
        case 63:
            send_char('/');
            break;
    }
}

void matrix_init_quantum() {
#ifdef BOOTMAGIC_LITE
    bootmagic_lite();
#endif
    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    }
#if defined(ORYX_ENABLE) && defined(DYNAMIC_KEYMAP_ENABLE)
    matrix_init_oryx();
#endif
#if defined(LED_NUM_LOCK_PIN) || defined(LED_CAPS_LOCK_PIN) || defined(LED_SCROLL_LOCK_PIN) || defined(LED_COMPOSE_PIN) || defined(LED_KANA_PIN)
    // TODO: remove calls to led_init_ports from keyboards and remove ifdef
    led_init_ports();
#endif
#ifdef BACKLIGHT_ENABLE
#    ifdef LED_MATRIX_ENABLE
    led_matrix_init();
#    else
    backlight_init_ports();
#    endif
#endif
#ifdef AUDIO_ENABLE
    audio_init();
#endif
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_init();
#endif
#if defined(UNICODE_ENABLE) || defined(UNICODEMAP_ENABLE) || defined(UCIS_ENABLE)
    unicode_input_mode_init();
#endif
#ifdef HAPTIC_ENABLE
    haptic_init();
#endif
#if defined(BLUETOOTH_ENABLE) && defined(OUTPUT_AUTO_ENABLE)
    set_output(OUTPUT_AUTO);
#endif

    matrix_init_kb();
}

void matrix_scan_quantum() {
#if defined(AUDIO_ENABLE)
    // There are some tasks that need to be run a little bit
    // after keyboard startup, or else they will not work correctly
    // because of interaction with the USB device state, which
    // may still be in flux...
    //
    // At the moment the only feature that needs this is the
    // startup song.
    static bool     delayed_tasks_run  = false;
    static uint16_t delayed_task_timer = 0;
    if (!delayed_tasks_run) {
        if (!delayed_task_timer) {
            delayed_task_timer = timer_read();
        } else if (timer_elapsed(delayed_task_timer) > 300) {
            audio_startup();
            delayed_tasks_run = true;
        }
    }
#endif

#if defined(AUDIO_ENABLE) && !defined(NO_MUSIC_MODE)
    matrix_scan_music();
#endif

#ifdef SEQUENCER_ENABLE
    matrix_scan_sequencer();
#endif

#ifdef TAP_DANCE_ENABLE
    matrix_scan_tap_dance();
#endif

#ifdef COMBO_ENABLE
    combo_task();
#endif

#ifdef LED_MATRIX_ENABLE
    led_matrix_task();
#endif

#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_task();
#endif

#ifdef WPM_ENABLE
    decay_wpm();
#endif

#ifdef HAPTIC_ENABLE
    haptic_task();
#endif

#ifdef DIP_SWITCH_ENABLE
    dip_switch_read(false);
#endif

#ifdef AUTO_SHIFT_ENABLE
    autoshift_matrix_scan();
#endif

    matrix_scan_kb();
}

#ifdef HD44780_ENABLED
#    include "hd44780.h"
#endif

// Functions for spitting out values
//

void send_dword(uint32_t number) {
    uint16_t word = (number >> 16);
    send_word(word);
    send_word(number & 0xFFFFUL);
}

void send_word(uint16_t number) {
    uint8_t byte = number >> 8;
    send_byte(byte);
    send_byte(number & 0xFF);
}

void send_byte(uint8_t number) {
    uint8_t nibble = number >> 4;
    send_nibble(nibble);
    send_nibble(number & 0xF);
}

void send_nibble(uint8_t number) { tap_code16(hex_to_keycode(number)); }

__attribute__((weak)) uint16_t hex_to_keycode(uint8_t hex) {
    hex = hex & 0xF;
    if (hex == 0x0) {
        return KC_0;
    } else if (hex < 0xA) {
        return KC_1 + (hex - 0x1);
    } else {
        return KC_A + (hex - 0xA);
    }
}

void api_send_unicode(uint32_t unicode) {
#ifdef API_ENABLE
    uint8_t chunk[4];
    dword_to_bytes(unicode, chunk);
    MT_SEND_DATA(DT_UNICODE, chunk, 5);
#endif
}

//------------------------------------------------------------------------------
// Override these functions in your keymap file to play different tunes on
// different events such as startup and bootloader jump

__attribute__((weak)) void startup_user() {}

__attribute__((weak)) void shutdown_user() {}

//------------------------------------------------------------------------------

#ifdef WEBUSB_ENABLE
__attribute__((weak)) bool webusb_receive_user(uint8_t *data, uint8_t length) { return false; }
__attribute__((weak)) bool webusb_receive_kb(uint8_t *data, uint8_t length) { return webusb_receive_user(data, length); }

bool webusb_receive_quantum(uint8_t *data, uint8_t length) {
#    ifdef ORYX_ENABLE
    return webusb_receive_oryx(data, length);
#    else
    return webusb_receive_kb(data, length);
#    endif
}
#endif
