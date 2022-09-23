#ifndef LEEP_COMBOS
#define LEEP_COMBOS

#include "tap_dance.c"
// TODO: eventually use dictionary stuff
// #include "g/keymap_combo.h"
// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_combo.md#dictionary-management

// Combos
enum combos {
  JI_ESC,
  JK_HYPHEN,
  JL_EQUALS,
  KL_ENTER,
  DF_QUOTE,
  SD_TICK,
  JCLN_LOCK,
  // SF is still an available combo slot.

  COMBO_LENGTH,
};
uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!

const uint16_t PROGMEM ji_combo[] = {KC_J, KC_I, COMBO_END};
const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM jl_combo[] = {KC_L, KC_J, COMBO_END};
const uint16_t PROGMEM kl_combo[] = {KC_L, KC_K, COMBO_END};
const uint16_t PROGMEM df_combo[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM sd_combo[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM jcln_combo[] = {KC_J, KC_SCLN, COMBO_END};

combo_t key_combos[COMBO_LENGTH] = {
  [JI_ESC] = COMBO(ji_combo, KC_ESCAPE),
  [JK_HYPHEN] = COMBO(jk_combo, KC_MINUS),
  [JL_EQUALS] = COMBO(jl_combo, KC_EQUAL),
  [KL_ENTER] = COMBO(kl_combo, KC_ENTER),
  [DF_QUOTE] = COMBO_ACTION(df_combo),
  [SD_TICK] = COMBO(sd_combo, KC_GRAVE),
  [JCLN_LOCK] = COMBO(jcln_combo, CK_LOCK),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!pressed) {
        return;
    }
    switch(combo_index) {
        case DF_QUOTE:
        // See below link for more info on get_mods and mod masks.
        // https://github.com/qmk/qmk_firmware/blob/master/docs/feature_advanced_keycodes.md
        if (get_mods() & MOD_MASK_SHIFT) {
            if (get_mods() & MOD_BIT(KC_LSFT)) {
                SEND_STRING(SS_UP(X_LSFT) "'" SS_DOWN(X_LSFT));
            } else if (get_mods() & MOD_BIT(KC_RSFT)) {
                SEND_STRING(SS_UP(X_RSFT) "'" SS_DOWN(X_RSFT));
            }
        } else {
            SEND_STRING("\"");
        }
        break;
    }
}

#endif
