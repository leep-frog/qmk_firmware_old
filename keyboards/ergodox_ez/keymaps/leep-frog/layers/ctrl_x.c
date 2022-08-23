#define LAYOUT_CTRL_X() = LAYOUT_ergodox_pretty(\
    RCTL(KC_EQL),  RCTL(KC_1),     RCTL(KC_2),    RCTL(KC_3),    RCTL(KC_4), RCTL(KC_5),    RCTL(WS_LEFT),     RCTL(CK_ESC),  RCTL(KC_6), RCTL(KC_7),  RCTL(KC_8),     RCTL(KC_9),        RCTL(KC_0),        TO(LR_SAFE),\
    RCTL(KC_TAB),  RCTL(KC_Q),     RCTL(KC_W),    RCTL(KC_E),    RCTL(KC_R), RCTL(KC_T),    KC_LEFT,           KC_RIGHT,      CK_PSTE,   RCTL(KC_U),  RCTL(KC_I),     RCTL(KC_O),        RCTL(KC_P),        RCTL(KC_BSLASH),\
    RCTL(KC_LSPO), RCTL(KC_A),     RCTL(KC_S),    RCTL(KC_D),    RCTL(KC_F), RCTL(KC_G),                                      RCTL(KC_H), RCTL(KC_J),  RCTL(KC_K),     RCTL(KC_L),        RCTL(KC_SCLN),     RCTL(KC_RSPC),\
    RCTL(KC_LCBR), RCTL(KC_Z),     RCTL(KC_X),    RCTL(KC_C),    RCTL(KC_V), RCTL(KC_B),    RCTL(WS_LEFT),     RCTL(WS_RGHT), RCTL(KC_N), RCTL(KC_M),  RCTL(KC_COMMA), RCTL(KC_DOT),      RCTL(KC_SLSH),     RCTL(KC_RCBR),\
    RCTL(KC_LGUI), RCTL(KC_QUOTE), LALT(KC_LSFT), RCTL(KC_LEFT), MO(LR_SYMB),                                                             RCTL(KC_UP), RCTL(KC_DOWN),  RCTL(KC_LBRACKET), RCTL(KC_RBRACKET), RCTL(KC_RGUI),\
\
                                                                             RCTL(WS_RGHT), RCTL(KC_HOME),     RCTL(KC_PGUP), RCTL(WS_LEFT),\
                                                                                             RCTL(KC_END),     RCTL(KC_PGDN),\
                                                   RCTL(KC_TAB), LT(LR_SYMB, KC_ENTER), LALT_T(KC_DELETE),     RCTL_T(KC_BSPC), RSFT_T(KC_SPACE), RGUI_T(KC_RCBR)\
)
