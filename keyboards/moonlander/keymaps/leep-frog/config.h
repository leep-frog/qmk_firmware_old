#pragma once

#define ORYX_CONFIGURATOR

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
