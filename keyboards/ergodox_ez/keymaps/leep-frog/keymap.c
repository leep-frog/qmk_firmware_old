#include QMK_KEYBOARD_H
#include "version.h"
#include "../../../../users/leep-frog/main.c"
#include "layers/main.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[LR_BASE] = LAYOUT_ergodox_pretty(
    CK_LOCK,  KC_1,     KC_2,    KC_3,    KC_4,    KC_5, WS_LEFT,        CK_ESC,       KC_6, KC_7,  KC_8,     KC_9,    KC_0,                    TO(LR_SAFE),
    KC_TAB,   KC_Q,     KC_W,    KC_E,    KC_R,    KC_T, LGHT_ON,    LGHT_OF, KC_Y, KC_U,  KC_I,     KC_O,    KC_P,                    KC_BSLASH,
    KC_LSPO,  KC_A,     KC_S,    KC_D,    KC_F,    KC_G,                               KC_H, KC_J,  KC_K,     KC_L,    LT(LR_OUTLOOK, KC_SCLN), KC_RSPC,
    KC_LCBR,  KC_Z,     KC_X,    KC_C,    KC_V,    KC_B, WS_LEFT,        WS_RGHT,      KC_N, KC_M,  KC_COMMA, KC_DOT,  KC_SLSH,                 KC_RCBR,
    KC_GRAVE, KC_QUOTE, KC_LGUI, KC_LEFT, KC_RIGHT,                                          KC_UP, KC_DOWN,  KC_LBRC, KC_RBRC,                 KC_LOCK,

                                                    WS_RGHT, KC_HOME,    KC_PGUP, WS_LEFT,
                                                              KC_END,    KC_PGDN,
LT(LR_SHORTCUTS, KC_TAB), LT(LR_SYMB, KC_ENTER), LT(LR_ALT, KC_DELETE),  LT(LR_CTRL, KC_BSPC), RSFT_T(KC_SPACE), LT(LR_NAVIGATION, KC_LGUI)
),

[LR_SAFE] = LAYOUT_ergodox_pretty(
    KC_GRAVE, KC_1,     KC_2,    KC_3,    KC_4,     KC_5, _______,        CK_ESC,       KC_6,    KC_7,    KC_8,        KC_9,    KC_0,    TO(LR_BASE),
    KC_TAB,   KC_Q,     KC_W,    KC_E,    KC_R,     KC_T, LGHT_ON,    LGHT_OF, KC_Y,    KC_U,    KC_I,        KC_O,    KC_P,    KC_BSLASH,
    KC_LSPO,  KC_A,     KC_S,    KC_D,    KC_F,     KC_G,                               KC_H,    KC_J,    KC_K,        KC_L,    KC_SCLN, KC_RSPC,
    KC_LSFT,  KC_Z,     KC_X,    KC_C,    KC_V,     KC_B, WS_LEFT,        WS_RGHT,      KC_N,    KC_M,    KC_COMMA,    KC_DOT,  KC_SLSH, KC_RCBR,
    KC_GRAVE, KC_QUOTE, _______, KC_LEFT, KC_RIGHT,                                              KC_UP,   KC_DOWN,     KC_LBRC, KC_RBRC, _______,

                                                 WS_RGHT, KC_HOME,        KC_PGUP, WS_LEFT,
                                                           KC_END,        KC_PGDN,
                                        KC_TAB, KC_ENTER, KC_RALT,        KC_RCTL, RSFT_T(KC_SPACE), KC_RGUI
),

[LR_CTRL_ALT] = LAYOUT_ergodox_pretty(
    KC_GRAVE, KC_1,     KC_2,    KC_3,    KC_4,     KC_5, _______,        CK_ESC,       KC_6,    KC_7,    KC_8,        KC_9,    KC_0,    TO(LR_BASE),
    KC_TAB,   KC_Q,     KC_W,    KC_E,    KC_R,     KC_T, LGHT_ON,    LGHT_OF, KC_Y,    KC_U,    KC_I,        KC_O,    KC_P,    KC_BSLASH,
    KC_LSPO,  KC_A,     KC_S,    KC_D,    KC_F,     KC_G,                               KC_H,    KC_J,    KC_K,        KC_L,    KC_SCLN, KC_RSPC,
    KC_LSFT,  KC_Z,     KC_X,    KC_C,    KC_V,     KC_B, WS_LEFT,        WS_RGHT,      KC_N,    KC_M,    KC_COMMA,    KC_DOT,  KC_SLSH, KC_RCBR,
    KC_GRAVE, KC_QUOTE, _______, KC_LEFT, KC_RIGHT,                                              KC_UP,   KC_DOWN,     KC_LBRC, KC_RBRC, _______,

                                                 WS_RGHT, KC_HOME,        KC_PGUP, WS_LEFT,
                                                           KC_END,        KC_PGDN,
                                        KC_TAB, KC_ENTER, _______,        _______, RSFT_T(KC_SPACE), KC_RGUI
),

[LR_CTRL] = LAYOUT_ergodox_pretty(
    RCTL(KC_EQL),  RCTL(KC_1),     RCTL(KC_2),     RCTL(KC_3),    RCTL(KC_4), RCTL(KC_5), RCTL(WS_LEFT),    RCTL(CK_ESC),  RCTL(KC_6), RCTL(KC_7),      RCTL(KC_8),     RCTL(KC_9),        RCTL(KC_0),        TO(LR_SAFE),
    RCTL(KC_TAB),          RCTL(KC_Q),     CTRL_W,         KC_END,        RCTL(KC_R), RCTL(KC_T), KC_LEFT,          KC_RIGHT,      RCTL(KC_Y), CK_TABB,         RCTL(KC_I),     CK_TABF,           KC_UP,             RCTL(KC_BSLASH),
    LSFT_T(RCTL(KC_LPRN)), KC_HOME,        RCTL(KC_F),     KC_DELETE,     KC_RIGHT,   CK_CTLG,                                   KC_BSPC,  TGL_SHF, CK_KILL,   KC_PGUP,           RCTL(KC_SCLN),     RSFT_T(RCTL(KC_RPRN)),
    RCTL(KC_LCBR),         RCTL(KC_Z),     OSL(LR_CTRL_X), CK_COPY,       KC_PGDOWN,  KC_LEFT,    KC_SCLN,          RCTL(WS_RGHT), KC_DOWN,    RCTL(KC_M),      RCTL(KC_COMMA), RCTL(KC_DOT),      RCTL(KC_SLSH),     RCTL(KC_RCBR),
    RCTL(KC_LGUI),         RCTL(KC_QUOTE), LALT(KC_LSFT),  RCTL(KC_LEFT), MO(LR_SYMB),                                                         RCTL(KC_UP),     RCTL(KC_DOWN),  RCTL(KC_LBRACKET), RCTL(KC_RBRACKET), RCTL(KC_RGUI),

                                                                           RCTL(WS_RGHT), RCTL(KC_HOME),    RCTL(KC_PGUP), RCTL(WS_LEFT),
                                                                                           RCTL(KC_END),    RCTL(KC_PGDN),
                                                          RCTL(KC_TAB), RCTL(KC_ENTER), MO(LR_CTRL_ALT),    _______,       RCTL(KC_SPACE), RGUI_T(KC_RCBR)
),

[LR_CTRL_X] = LAYOUT_ergodox_pretty(
    RCTL(KC_EQL),  RCTL(KC_1),     RCTL(KC_2),    RCTL(KC_3),    RCTL(KC_4), RCTL(KC_5),    RCTL(WS_LEFT),     RCTL(CK_ESC),  RCTL(KC_6), RCTL(KC_7),  RCTL(KC_8),     RCTL(KC_9),        RCTL(KC_0),        TO(LR_SAFE),
    RCTL(KC_TAB),  RCTL(KC_Q),     RCTL(KC_W),    RCTL(KC_E),    RCTL(KC_R), RCTL(KC_T),    KC_LEFT,           KC_RIGHT,      CK_PSTE,   RCTL(KC_U),  RCTL(KC_I),     RCTL(KC_O),        RCTL(KC_P),        RCTL(KC_BSLASH),
    RCTL(KC_LSPO), RCTL(KC_A),     RCTL(KC_S),    RCTL(KC_D),    RCTL(KC_F), RCTL(KC_G),                                      RCTL(KC_H), RCTL(KC_J),  RCTL(KC_K),     RCTL(KC_L),        RCTL(KC_SCLN),     RCTL(KC_RSPC),
    RCTL(KC_LCBR), RCTL(KC_Z),     RCTL(KC_X),    RCTL(KC_C),    RCTL(KC_V), RCTL(KC_B),    RCTL(WS_LEFT),     RCTL(WS_RGHT), RCTL(KC_N), RCTL(KC_M),  RCTL(KC_COMMA), RCTL(KC_DOT),      RCTL(KC_SLSH),     RCTL(KC_RCBR),
    RCTL(KC_LGUI), RCTL(KC_QUOTE), LALT(KC_LSFT), RCTL(KC_LEFT), MO(LR_SYMB),                                                             RCTL(KC_UP), RCTL(KC_DOWN),  RCTL(KC_LBRACKET), RCTL(KC_RBRACKET), RCTL(KC_RGUI),

                                                                             RCTL(WS_RGHT), RCTL(KC_HOME),     RCTL(KC_PGUP), RCTL(WS_LEFT),
                                                                                             RCTL(KC_END),     RCTL(KC_PGDN),
                                                   RCTL(KC_TAB), LT(LR_SYMB, KC_ENTER), LALT_T(KC_DELETE),     RCTL_T(KC_BSPC), RSFT_T(KC_SPACE), RGUI_T(KC_RCBR)
),

[LR_ALT] = LAYOUT_ergodox_pretty(
    RALT(KC_EQL),          RALT(KC_1),     RALT(KC_2),    RALT(KC_3),      RALT(KC_4),     RALT(KC_5),    RALT(WS_LEFT),     RALT(CK_ESC),  RALT(KC_6),             RALT(KC_7),              RALT(KC_8),     RALT(KC_9),        RALT(KC_0),        TO(LR_SAFE),
    RALT(KC_TAB),          RALT(KC_Q),     RALT(KC_F4),   RALT(KC_E),      RALT(KC_R),     CK_ALTT,      KC_LEFT,           KC_RIGHT,      CK_PSTE,               TGL_SLT, RALT(KC_I),     TGL_ALT, RALT(KC_P),        RALT(KC_BSLASH),
    LSFT_T(RALT(KC_LPRN)), KC_HOME,        RALT(KC_S),    RCTL(KC_DELETE), RCTL(KC_RIGHT), RALT(KC_G),                                      CK_UNBS, RALT(KC_J),              RALT(KC_K),     KC_WH_U,           RALT(KC_SCLN),     RSFT_T(RALT(KC_RPRN)),
    RALT(KC_LCBR),         RALT(KC_Z),     RALT(KC_X),    RALT(KC_C),      KC_WH_D,        RCTL(KC_LEFT), RALT(WS_LEFT),     RALT(WS_RGHT), RALT(KC_N),             RALT(KC_M),              RALT(KC_COMMA), RALT(KC_DOT),      RALT(KC_SLSH),     RALT(KC_RCBR),
    RALT(KC_LGUI),         RALT(KC_QUOTE), LALT(KC_LSFT), RALT(KC_LEFT),   MO(LR_SYMB),                                                                             RALT(KC_UP),             RALT(KC_DOWN),  RALT(KC_LBRACKET), RALT(KC_RBRACKET), RALT(KC_RGUI),

                                                                                      RALT(WS_RGHT),  RALT(KC_HOME),         RALT(KC_PGUP),   RALT(WS_LEFT),
                                                                                                      RALT(KC_END),          RALT(KC_PGDN),
                                                                        RALT(KC_TAB), RALT(KC_ENTER), _______,               MO(LR_CTRL_ALT), RALT(KC_SPACE), RGUI_T(KC_RCBR)
),

// Shortcuts and google docs
[LR_SHORTCUTS] = LAYOUT_ergodox_pretty(
    _______,  _______,    _______,       _______,           _______,        _______, _______,     _______, _______,      _______,     _______,        _______, _______, TO(LR_SAFE),
    _______,  RESET,      CK_WWWB,       MS_MID,   CK_WWWF,        _______, _______,     _______, URL_PST, URL_COPY, URL_ICP, CK_MOMA, CK_CL,   _______,
    KC_ENTER, RCTL(KC_A), MS_LEFT, MS_SMID, MS_RGHT, _______,                       _______,      GD_HD_4,     GD_HD_5,        KC_WH_U, _______, _______,
    _______,  CK_MCR2, CK_MCR1,    MS_CTRL,     KC_WH_D,        GD_BULT, _______,     _______, CK_NEW, GD_HD_1,     GD_HD_2,        GD_HD_3, _______, _______,
    _______,  _______,    _______,       _______,           _______,                                                     _______,     _______,        _______, _______, _______,

                                                                        _______, _______,     _______, _______,
                                                                                 _______,     _______,
                                                               _______, _______, _______,     _______, TGL_SHF, _______
),

[LR_SYMB] = LAYOUT_ergodox_pretty(
    VRSN,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,     _______, KC_F6,   KC_F7,   KC_F8,  KC_F9, KC_F10,  TO(LR_SAFE),
    _______, KC_EXLM, KC_COLN, KC_EQL,  KC_CIRC, KC_LPRN, _______,     _______, KC_RPRN, KC_7,    KC_8,   KC_9,  KC_PERC, KC_F12,
    _______, KC_AMPR, KC_ASTR, KC_DLR,  KC_SLSH, KC_LBRC,             CK_MARKDOWN_PASTE, KC_4,    KC_5,   KC_6,  KC_0,    KC_PERC,
    _______, KC_TILD, KC_EXLM, KC_AT,   KC_EQL,  KC_LCBR, _______,     _______, KC_RCBR, KC_1,    KC_2,   KC_3,  KC_0,    _______,
    EEP_RST, _______, _______, _______, _______,                                         _______, KC_DOT, KC_0,  KC_EQL,  _______,

                                                 RGB_MOD, _______,     RGB_TOG, _______,
                                                          _______,     _______,
                                        RGB_VAD, RGB_VAI, _______,     _______, KC_SPACE, RGB_HUI
),

[LR_NAVIGATION] = LAYOUT_ergodox_pretty(
    _______, _______, _______, _______, _______,        _______,          _______,     _______, _______,          _______, _______,    _______, _______,     TO(LR_SAFE),
    _______, _______, CK_WWWB, _______, RESET,          RCTL(RALT(KC_T)), _______,     _______, RCTL(KC_T),       CK_TABB, RCTL(KC_W), CK_TABF, RGUI(KC_UP), TGL_ALT,
    _______, _______, _______, _______, RGUI(KC_RIGHT), _______,                                RCTL(RSFT(KC_T)), WS_LEFT, CODE_WS,    WS_RGHT, MISC_WS,     TGL_ELT,
    _______, _______, _______, _______, _______,        RGUI(KC_LEFT),    _______,     _______, RGUI(KC_DOWN),    _______, WS_UP,      WS_DOWN, _______,     _______,
    _______, _______, _______, _______, _______,                                                _______,          _______, _______,    _______, _______,

                                                          _______, _______,     _______, _______,
                                                                   _______,     _______,
                                            RGUI(KC_TAB), _______, _______,     _______, _______, _______
),

[LR_OUTLOOK] = LAYOUT_ergodox_pretty(
    _______, _______, _______, _______, _______, _______, _______, /**/ _______, _______,    _______,    _______,        _______,           _______, TO(LR_SAFE),
    _______, _______, _______, _______, _______, _______, _______, /**/ _______, RCTL(KC_U), RCTL(KC_Q), RCTL(KC_COMMA), CK_OLRL, _______, _______,
    _______, _______, _______, _______, _______, _______,          /**/          _______,    RCTL(KC_1), RCTL(KC_DOT),   RCTL(KC_2),        _______, _______,
    _______, _______, _______, _______, _______, _______, _______, /**/ _______, _______,    RCTL(KC_U), _______,        RCTL(KC_M),        _______, _______,
    _______, _______, _______, _______, _______,                   /**/                      _______,    _______,        _______,           _______, _______,

                                                 _______, _______, /**/ _______, _______,
                                                          _______, /**/ _______,
                                        _______, _______, _______, /**/ _______, _______, _______
),
};
