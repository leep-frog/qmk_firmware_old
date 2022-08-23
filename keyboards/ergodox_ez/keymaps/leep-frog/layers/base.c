#define BASE_LAYOUT() LAYOUT_ergodox_pretty(\
    CK_LOCK,  KC_1,     KC_2,    KC_3,    KC_4,    KC_5, WS_LEFT,        CK_ESC,       KC_6, KC_7,  KC_8,     KC_9,    KC_0,                    TO(LR_SAFE),\
    KC_TAB,   KC_Q,     KC_W,    KC_E,    KC_R,    KC_T, LGHT_ON,    LGHT_OF, KC_Y, KC_U,  KC_I,     KC_O,    KC_P,                    KC_BSLASH,\
    KC_LSPO,  KC_A,     KC_S,    KC_D,    KC_F,    KC_G,                               KC_H, KC_J,  KC_K,     KC_L,    LT(LR_OUTLOOK, KC_SCLN), KC_RSPC,\
    KC_LCBR,  KC_Z,     KC_X,    KC_C,    KC_V,    KC_B, WS_LEFT,        WS_RGHT,      KC_N, KC_M,  KC_COMMA, KC_DOT,  KC_SLSH,                 KC_RCBR,\
    KC_GRAVE, KC_QUOTE, KC_LGUI, KC_LEFT, KC_RIGHT,                                          KC_UP, KC_DOWN,  KC_LBRC, KC_RBRC,                 KC_LOCK,\
\
                                                    WS_RGHT, KC_HOME,    KC_PGUP, WS_LEFT,\
                                                              KC_END,    KC_PGDN,\
LT(LR_SHORTCUTS, KC_TAB), LT(LR_SYMB, KC_ENTER), LT(LR_ALT, KC_DELETE),  LT(LR_CTRL, KC_BSPC), RSFT_T(KC_SPACE), LT(LR_NAVIGATION, KC_LGUI)\
)
