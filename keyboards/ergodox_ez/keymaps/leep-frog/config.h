#ifndef LEEP_CONFIG_H
#define LEEP_CONFIG_H

#include "../../config.h"

// How long it takes for a tap to become a hold. Primarily added because
// Shift-combinations were too quick and causing a parentheses to be added.
#ifdef TAPPING_TERM
#    undef TAPPING_TERM
#    define TAPPING_TERM 150
#endif
// Allows us to define tapping term per key. See link for more details:
// https://github.com/qmk/qmk_firmware/blob/master/docs/tap_hold.md#tapping-term
#define TAPPING_TERM_PER_KEY

// Combo setup
// #define FORCE_NKRO
#define COMBO_TERM 40  // Number of milliseconds for combo keys.

// Mouse settings
/*#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 9

#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 40

#undef MOUSEKEY_WHEEL_MAX_SPEED
#define MOUSEKEY_WHEEL_MAX_SPEED 4

#undef MOUSEKEY_WHEEL_TIME_TO_MAX
#define MOUSEKEY_WHEEL_TIME_TO_MAX 0
*/

#define COMBO_VARIABLE_LEN

#endif
