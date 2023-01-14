#ifndef LEEP_FEATURE_SYMBOL_LAYER_OVERLAP
#define LEEP_FEATURE_SYMBOL_LAYER_OVERLAP

/* Issue: when typing fast, would want to type " d". However, the space
key is also the symbol layer, so the following would happen:

- Press symbol key (for space presumable)
- Press another key (e.g. d)
- Unpress symbol key
- Unpress another key

This results in sending "$" when we actually wanted to send " d".

This feature file implements a fix for this issue.

Test cases:
1. See above sequence
2. Press a key in the symbol layer
*/

// Logic for stuff
static keypos_t first_symb_press_key      = {};
static bool     first_symb_press          = false;
static bool     resolved_first_symb_press = true;

void SymbolLayerOverlap_reset(void) {
    first_symb_press          = false;
    resolved_first_symb_press = true;
}

bool SymbolLayerOverlap_handled(uint16_t keycode, keyrecord_t *record) {
    // Need to ensure we don't check the symbol key itself.
    // We can do a more accurate check, but the following is simple enough
    // and works for all keys for which the main issue is occurring.
    if (keycode > QK_MODS_MAX) {
        return false;
    }

    if (IS_LAYER_ON(LR_SYMB)) {
        // Record the first key press in the symbol layer, but don't actually press it.
        if (!first_symb_press && record->event.pressed) {
            first_symb_press          = true;
            first_symb_press_key      = ((keypos_t){
                     .col = record->event.key.col,
                     .row = record->event.key.row,
            });
            resolved_first_symb_press = false;
            return true;
        } else if (first_symb_press && !record->event.pressed && !resolved_first_symb_press) {
            // We're unpressing the key while still in the symbol layer
            resolved_first_symb_press = true;
            tap_code16(keymap_key_to_keycode(get_highest_layer(layer_state), first_symb_press_key));
            return true;
        }
    } else {
        if (!record->event.pressed && !resolved_first_symb_press) {
            // We're unpressing the key in a different layer. Need to send space,
            // since the symb/space key will have only registered as symbol layer key
            // and not the space key.
            resolved_first_symb_press = true;
            SEND_STRING(" ");
            tap_code16(keymap_key_to_keycode(get_highest_layer(layer_state), first_symb_press_key));
        }
    }
    return false;
}

#endif
