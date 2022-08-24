#pragma once

#define ORYX_CONFIGURATOR

#include "../../config.h"

// How long it takes for a tap to become a hold. Primarily added because
// Shift-combinations were too quick and causing a parentheses to be added.
#ifdef TAPPING_TERM
#undef TAPPING_TERM
#define TAPPING_TERM 150
#endif
// Allows us to define tapping term per key. See link for more details:
// https://github.com/qmk/qmk_firmware/blob/master/docs/tap_hold.md#tapping-term
#define TAPPING_TERM_PER_KEY

// Combo setup
//#define FORCE_NKRO
#define COMBO_TERM 40  // Number of milliseconds for combo keys.

#define COMBO_VARIABLE_LEN

#define RGBLIGHT_LAYERS

#define ENABLE_RGB_MATRIX_BREATHING

#define FE_1(M, X) M(X)
#define FE_2(M, X, ...) M(X), FE_1(M, __VA_ARGS__)
#define FE_3(M, X, ...) M(X), FE_2(M, __VA_ARGS__)
#define FE_4(M, X, ...) M(X), FE_3(M, __VA_ARGS__)
#define FE_5(M, X, ...) M(X), FE_4(M, __VA_ARGS__)
#define FE_6(M, X, ...) M(X), FE_5(M, __VA_ARGS__)
#define FE_7(M, X, ...) M(X), FE_6(M, __VA_ARGS__)
#define FE_8(M, X, ...) M(X), FE_7(M, __VA_ARGS__)
#define FE_9(M, X, ...) M(X), FE_8(M, __VA_ARGS__)
#define FE_10(M, X, ...) M(X), FE_9(M, __VA_ARGS__)
#define FE_11(M, X, ...) M(X), FE_10(M, __VA_ARGS__)
#define FE_12(M, X, ...) M(X), FE_11(M, __VA_ARGS__)

#define STARTUP_SONG SONG(ZELDA_DISCOVERY)
#define GOODBYE_SONG SONG(ZELDA_SPIRIT_ORB)
