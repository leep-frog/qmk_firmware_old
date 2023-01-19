#include QMK_KEYBOARD_H
#include "version.h"

// interface.c requirements
#define LEEP_SAFE_RANGE ML_SAFE_RANGE

#include "../../../../users/leep-frog/main.c"

// Can't evaluate macro in macro, so use this to ignore bottom row of keyboard
// https://stackoverflow.com/questions/35114050/is-there-a-way-to-force-c-preprocessor-to-evaluate-macro-arguments-before-the-ma
#define ML_LAYOUT(...) LAYOUT_moonlander(__VA_ARGS__)
#define BOTTOM_ROW CK_MUT1, CK_MUT2, _______, KC_LALT, KC_LCTL, CK_MCR1, CK_MCR2, KC_RCTL, KC_RALT, _______, CK_EYE, KB_OFF
// The tap dances defined for TO_SFT and TO_SYMB require that those keys are at the
// same spot in all layers. See the tap_dance.c file for more info.
#define THUMB_ROW(LEFT_MIDDLE, LEFT_RIGHT, RIGHT_LEFT, RIGHT_MIDDLE) TO_SFT, LEFT_MIDDLE, LEFT_RIGHT, RIGHT_LEFT, RIGHT_MIDDLE, TO_SYMB

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LR_BASE] = ML_LAYOUT(
        CK_LOCK, KC_1, KC_2, KC_3, KC_4, KC_5, WS_LEFT,       CK_ESC,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    TO_SFTY,
        _______, KC_Q, KC_W, KC_E, KC_R, KC_T, LGHT_ON,       LGHT_OF, TD_Y, TD_U, TD_I,    KC_O,   KC_P,    _______,
        KC_LSPO, TD_A, KC_S, KC_D, KC_F, KC_G, WS_LEFT,       WS_RGHT, KC_H, KC_J, KC_K,    KC_L,   TO_OTLK, KC_RSPC,
        KC_LCBR, KC_Z, KC_X, TD_C, TD_V, TD_B,                         KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCBR,
                                                     BOTTOM_ROW,

                              THUMB_ROW(TO_ALT, TO_SHCT,       TO_NAV, TO_CTRL)
    ),

    [LR_SAFE] = ML_LAYOUT(
        CK_LOCK, KC_1, KC_2, KC_3, KC_4, KC_5, _______,       CK_ESC,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    TO(LR_BASE),
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, LGHT_ON,       LGHT_OF, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
        KC_LSPO, KC_A, KC_S, KC_D, KC_F, KC_G, WS_LEFT,       WS_RGHT, KC_H, KC_J, KC_K,    KC_L,   TO_OTLK, KC_RSPC,
        KC_LCBR, KC_Z, KC_X, KC_C, KC_V, KC_B,                         KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCBR,

                                                     BOTTOM_ROW,

                                THUMB_ROW(TO_ALT, TO_SHCT,   TO_NAV, TO_CTRL)
    ),

    [LR_CTRL] = ML_LAYOUT(
        CL(EQL),  CL(1),   CL(2),   CL(3),   CL(4),   CL(5),   RCTL(WS_LEFT),  CK_ESC,  CL(6),   CL(7),   CL(8),   CL(9),    CL(0),    TO_SFTY,
        CL(TAB),  CL(Q),   CTRL_W,  KC_END,  CL(R),   TD_CTLT, KC_LEFT,        KC_RGHT, CL(Y),   CK_TABB, CL(I),   CK_TABF,  KC_UP,    CL(BSLS),
LSFT_T(CL(LPRN)), KC_HOME, CL(F),   KC_DEL,  KC_RGHT, CK_CTLG, _______,        _______, KC_BSPC, TGL_SHF, CK_KILL, KC_PGUP,  CL(SCLN), RSFT_T(CL(RPRN)),
        CL(LCBR), CL(Z),   TO_CTLX, CK_COPY, KC_PGDN, KC_LEFT,                                   KC_DOWN, CL(M),   CL(COMM), CL(DOT),  CK_UNDO, CL(RCBR),

                                                                    BOTTOM_ROW,

                                         CL(ENTER), CL(TAB), TO_CTAL,         CL(RGUI), _______, CL(SPACE)
    ),

    [LR_CTRL_X] = ML_LAYOUT(
        _______,  CL(1), CL(2), CL(3), CL(4), CL(5), RCTL(WS_LEFT),           RCTL(CK_ESC),      CL(6), CL(7), CL(8),     CL(9),    CL(0),    TO_SFTY,
        CL(TAB),  CL(Q), CL(W), CL(E), CL(R), CL(T), KC_LEFT,                 KC_RIGHT, CK_PSTE, CL(U), CL(I), CL(O),     CL(P),    CL(BSLS),
        CL(LSPO), CL(A), CL(S), CL(D), CL(F), CL(G), RCTL(WS_LEFT),           RCTL(WS_RGHT),     CL(H), CL(J), CL(K),     CL(L),    CL(SCLN), CL(RSPC),
        CL(LCBR), CL(Z), CL(X), CL(C), CL(V), CL(B),                                             CL(N), CL(M), CL(COMMA), CL(DOT),  CL(SLSH), CL(RCBR),

                                                                   BOTTOM_ROW,

                                        CL(ENTER), CL(TAB), _______,        CL(RGUI), _______, CL(SPACE)
    ),

    [LR_ALT] = ML_LAYOUT(
        AL(EQL),  AL(1), AL(2),  AL(3),   AL(4),     AL(5),   RALT(WS_LEFT),        RALT(CK_ESC),  AL(6),   AL(7),   AL(8),    AL(9),   AL(0),    TO_SFTY,
        AL(TAB),  AL(Q), AL(F4), AL(E),   AL(R),     CK_ALTT, KC_LEFT,              KC_RIGHT,      CK_PSTE, CK_SATB, AL(I),    CK_ATB,  AL(P),    AL(BSLS),
LSFT_T(AL(LPRN)), AL(A), AL(S),  CL(DEL), CL(RIGHT), AL(G),   RALT(WS_LEFT),        RALT(WS_RGHT), CK_UNBS, AL(J),   AL(K),    KC_WH_U, AL(SCLN), RSFT_T(AL(RPRN)),
        AL(LCBR), AL(Z), AL(X),  AL(C),   KC_WH_D,   CL(LEFT),                                     AL(N),   AL(M),   AL(COMM), AL(DOT), AL(SLSH), AL(RCBR),

                                                                            BOTTOM_ROW,

                                                  AL(ENTER), _______, AL(TAB),       AL(RGUI), TO_CTAL, AL(SPACE)
    ),

    // This layer is basically identical to the safe layer aside from the thumb keys.
    [LR_CTRL_ALT] = ML_LAYOUT(
        CK_LOCK, KC_1, KC_2, KC_3, KC_4, KC_5, _______,       CK_ESC,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    TO_SFTY,
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, LGHT_ON,       LGHT_OF, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
        KC_LSPO, KC_A, KC_S, KC_D, KC_F, KC_G, WS_LEFT,       WS_RGHT, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_RSPC,
        KC_LCBR, KC_Z, KC_X, KC_C, KC_V, KC_B,                         KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCBR,

                                                      BOTTOM_ROW,

                             KC_ENTER, KC_TAB, _______,        KC_RGUI, _______, KC_SPACE
    ),

   [LR_SHORTCUTS] = ML_LAYOUT(
        _______,  _______, _______, _______, _______, _______, _______,           _______, _______, _______,  _______, _______, _______, TO_SFTY,
        _______,  _______, CK_WWWB, MS_MID,  CK_WWWF, _______, _______,           _______, KC_WH_U, URL_COPY, KC_MS_U, CK_MOMA, CK_CL,   _______,
        KC_ENTER, _______, CK_LOGS, KC_ACL1, KC_ACL0, _______, _______,           _______, KC_WH_D, KC_MS_L,  KC_MS_D, KC_MS_R, KC_RCTL, _______,
        _______,  _______, _______, MS_CTRL, _______, GD_BULT,                             CK_NEW,  GD_HD_1,  GD_HD_2, GD_HD_3, _______, _______,

                                                                     BOTTOM_ROW,

                                           KC_ACL1, KC_ACL0, KC_ACL2,           KC_MS_BTN3, KC_MS_BTN1, KC_MS_BTN2
    ),

    [LR_NAVIGATION] = ML_LAYOUT(
        _______, _______, _______, _______, _______,  _______,     _______,           _______, _______,  _______, _______, _______, _______, TO_SFTY,
        _______, _______, CK_WWWB, _______, TD_RST,   RCTL(AL(T)), _______,           _______, SLK_UP,   SLK_UP,  URL_CRI, CK_TABF, GU(UP),  CK_ATB,
        _______, _______, CK_SNIP, _______, GU(RGHT), _______,     _______,           _______, SLK_DWN,  SLK_DWN, SLK_JMP, CK_LOGS, MISC_WS, _______,
        _______, _______, _______, _______, _______,  GU(LEFT),                                GU(DOWN), _______, WS_UP,   WS_DOWN, _______, _______,

                                                                      BOTTOM_ROW,

                                            GU(TAB), GU(TAB), _______,           _______, GU(TAB), _______
    ),

    [LR_SYMB] = ML_LAYOUT(
        _______, KC_F1,   KC_F2,   KC_HASH, KC_F4,   KC_F5,   _______,           _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  TO_SFTY,
        _______, KC_EXLM, KC_COLN, KC_EQL,  KC_CIRC, KC_PIPE, _______,           _______, KC_RPRN, KC_7,    KC_8,    KC_9,    KC_PERC, KC_F12,
        KC_TAB,  KC_AMPR, KC_ASTR, KC_DLR,  KC_SLSH, KC_LBRC, _______,           _______, CK_MDPS, KC_4,    KC_5,    KC_6,    KC_0,    KC_PERC,
        _______, KC_TILD, KC_EXLM, KC_AT,   KC_EQL,  KC_BSLS,                             KC_RCBR, KC_1,    KC_2,    KC_3,    KC_0,    _______,
                                                                      BOTTOM_ROW,
                                            THUMB_ROW(KC_ENTER, RGB_HUI, TOGGLE_LAYER_COLOR, KC_SPACE)
    ),

    [LR_OUTLOOK] = ML_LAYOUT(
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______,   _______, _______, TO_SFTY,
        _______, _______, _______, SLK_UP,  _______, _______, _______,           _______, CL(U),   CL(Q),   CL(COMMA), OL_RLD,  _______, _______,
        _______, _______, _______, SLK_DWN, SLK_JMP, _______, _______,           _______, KC_DEL,  CL(1),   CL(DOT),   CL(2),   _______, _______,
        _______, _______, _______, _______, _______, _______,                             OL_TDAY, CL(U),   _______,   CL(M),   _______, _______,

                                                                      BOTTOM_ROW,

                                            _______, _______, _______,           _______, OL_LEFT, OL_RGHT
    ),

    [LR_ONE_HAND_LEFT] = ML_LAYOUT(
        _______, _______, _______, TD_RST,  _______, _______, _______,           _______, _______, _______,  _______, _______, _______, TO_SFTY,
       KC_TAB,   KC_BSPC, CL(W),   CK_SATB, CK_ATB,  TD_CTLT, _______,           _______, KC_WH_U, URL_COPY, KC_MS_U, _______, _______, _______,
       KC_ENTER, WS_LEFT, CL(R),   CK_TABB, CK_TABF, WS_RGHT, _______,           _______, KC_WH_D, KC_MS_L,  KC_MS_D, KC_MS_R, KC_RCTL, _______,
        _______, CK_WWWB, CK_WWWF, OH_COPY, OH_PSTE, _______,                             _______, KC_BTN1,  KC_BTN3, KC_BTN2, _______, _______,

                                                                      BOTTOM_ROW,

                                            _______, _______, _______,           CK_ACL, CK_ACL, CK_ACL
    ),

    [LR_ONE_HAND_RIGHT] = ML_LAYOUT(
        _______, _______, _______, _______, _______,  _______, _______,           _______, _______, _______, TD_RST,  _______, _______, TO_SFTY,
        _______, _______, _______, KC_MS_U, URL_COPY, KC_WH_U, _______,           _______, CL(W),   CK_SATB, CK_ATB,  TD_CTLT, KC_BSPC, KC_TAB,
        _______, KC_RCTL, KC_MS_L, KC_MS_D, KC_MS_R,  KC_WH_D, _______,           _______, WS_LEFT, CK_TABB, CK_TABF, CL(R),   WS_RGHT, KC_ENTER,
        _______, _______, KC_BTN2, KC_BTN3, KC_BTN1,  _______,                             _______, OH_COPY, OH_PSTE, CK_WWWB, CK_WWWF, _______,

                                                                      BOTTOM_ROW,

                                                CK_ACL, CK_ACL, CK_ACL,           _______, _______, _______
    ),

    /*
    [EMPTY_LAYER] = ML_LAYOUT(
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______, TO_SFTY,
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                             _______, _______, _______, _______, _______, _______,

                                                                      BOTTOM_ROW,

                                            _______, _______, _______,           _______, _______, _______
    ),
    */
};
