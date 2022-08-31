#include QMK_KEYBOARD_H
#include "version.h"

#define LEEP_SAFE_RANGE ML_SAFE_RANGE

//#define LEEP_LAYER_COLOR(lyr) rgb_matrix_set_color_all(layer_colors[lyr][0], layer_colors[lyr][1], layer_colors[lyr][2])
#define LEEP_LAYER_COLOR(lyr) rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR); rgblight_sethsv(layer_colors[lyr][0], layer_colors[lyr][1], layer_colors[lyr][2])
#define LEEP_SOLID_COLOR(clr) rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR); rgblight_sethsv(clr)
#define LEEP_COLOR_MODE(clr, mde) rgb_matrix_mode(mde); rgblight_sethsv(clr)

#include "../../../../users/leep-frog/main.c"

// Color on shift
int layer_colors[NUM_LAYERS][3] = {
  [0 ... NUM_LAYERS - 1] = { HSV_GREEN },
  [LR_BASE] = { HSV_CYAN },
  [LR_SAFE] = { HSV_GREEN },
  [LR_CTRL] = { HSV_SPRINGGREEN },
  [LR_ALT] = { HSV_MAGENTA },
  [LR_CTRL_X] = { HSV_CHARTREUSE },
  [LR_CTRL_ALT] = { HSV_GOLD },
  [LR_NAVIGATION] = { HSV_CORAL },
  [LR_SHORTCUTS] = { HSV_TURQUOISE },
  [LR_SYMB] = { HSV_ORANGE },
  [LR_OUTLOOK] = { HSV_BLUE },
  // We don't change the keyboard color for shift because there isn't
  // a way to only do it on mod and not on tap as well, and seeing the
  // color change for every space character was really annoying.
};

// Explicitly only define each song once (not once per use) to limit memory usage.
// All song uses should exclusively be used through references to these
// existing songs (and not by creating new song instances).
DEFINE_SONG(zelda_discovery_song, SONG(ZELDA_DISCOVERY));
DEFINE_SONG(zelda_guardian_battle_song, SONG(ZELDA_GUARDIAN_BATTLE));
DEFINE_SONG(zelda_item_found_song, SONG(ZELDA_ITEM_FOUND));
DEFINE_SONG(zelda_spirit_orb_song, SONG(ZELDA_SPIRIT_ORB));

DEFINE_SONG(mario_1_up_song, SONG(MARIO_1_UP));
DEFINE_SONG(mario_lost_a_life_song, SONG(MARIO_LOST_A_LIFE));
DEFINE_SONG_WITH_TEMPO(mario_game_over_song, SONG(MARIO_GAME_OVER), 100);

#define SNG_RESET LEEP_PLAY_SONG(mario_game_over_song)
#define SNG_MUTE LEEP_PLAY_SONG(mario_lost_a_life_song)
#define SNG_UNMUTE LEEP_PLAY_SONG(mario_1_up_song)
#define SNG_REC_START LEEP_PLAY_SONG(zelda_guardian_battle_song)
#define SNG_REC_1_END LEEP_PLAY_SONG(zelda_item_found_song)
#define SNG_REC_2_END LEEP_PLAY_SONG(mario_1_up_song)
#define SNG_REC_1_PLAY LEEP_PLAY_SONG(zelda_spirit_orb_song)
#define SNG_REC_2_PLAY LEEP_PLAY_SONG(zelda_discovery_song)

// Interface functions
void on_layer_change(uint8_t layer) {
  if (!recording && !shift_toggled) {
    LEEP_LAYER_COLOR(layer);
  }
}

bool on_reset(void) {
  SNG_RESET;
  while (is_playing_notes()) {
    wait_ms(150);
  }
  return true;
}

void on_mute_1(void) {
  SNG_MUTE;
}

void on_unmute_1(void) {
  SNG_UNMUTE;
}

void on_mute_2(void) {
  LEEP_SOLID_COLOR(HSV_ORANGE);
}

void on_unmute_2(void) {
  LEEP_SOLID_COLOR(HSV_GREEN);
}

void on_shift(void) {
  LEEP_COLOR_MODE(HSV_RED, RGB_MATRIX_CYCLE_PINWHEEL);
}

void on_unshift(void) {
  LEEP_LAYER_COLOR(LR_CTRL);
}

void recording_start(bool macro_1) {
  SNG_REC_START;
  LEEP_COLOR_MODE(HSV_BLUE, RGB_MATRIX_RAINBOW_PINWHEELS);
}

void recording_end(bool macro_1) {
  LEEP_LAYER_COLOR(LR_SHORTCUTS);
  if (macro_1) {
    SNG_REC_1_END;
  } else {
    SNG_REC_2_END;
  }
}

void recording_play(bool macro_1) {
  if (macro_1) {
    SNG_REC_1_PLAY;
  } else {
    SNG_REC_2_PLAY;
  }
}

// Can't evaluate macro in macro, so use this to ignore bottom row of keyboard
// https://stackoverflow.com/questions/35114050/is-there-a-way-to-force-c-preprocessor-to-evaluate-macro-arguments-before-the-ma
#define ML_LAYOUT(...) LAYOUT_moonlander(__VA_ARGS__)
#define BOTTOM_ROW CK_MUT1, CK_MUT2, _______, _______, _______, _______,    _______, _______, _______, _______, _______, RGB_TOG

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LR_BASE] = ML_LAYOUT(
        CK_LOCK, KC_1, KC_2, KC_3, KC_4, KC_5, WS_LEFT,       CK_ESC,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    TO_SFTY,
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, LGHT_ON,       LGHT_OF, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
        KC_LSPO, KC_A, KC_S, KC_D, KC_F, KC_G, WS_LEFT,       WS_RGHT, KC_H, KC_J, KC_K,    KC_L,   TO_OTLK, KC_RSPC,
        KC_LCBR, KC_Z, KC_X, KC_C, KC_V, KC_B,                         KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCBR,
                                                   BOTTOM_ROW,

                              TO_SYMB, TO_ALT, TO_SHCT,   TO_NAV, TO_CTL, TO_SFT
    ),

    [LR_SAFE] = ML_LAYOUT(
        CK_LOCK, KC_1, KC_2, KC_3, KC_4, KC_5, _______,       CK_ESC,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    TO(LR_BASE),
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, LGHT_ON,       LGHT_OF, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
        KC_LSPO, KC_A, KC_S, KC_D, KC_F, KC_G, WS_LEFT,       WS_RGHT, KC_H, KC_J, KC_K,    KC_L,   TO_OTLK, KC_RSPC,
        KC_LCBR, KC_Z, KC_X, KC_C, KC_V, KC_B,                         KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCBR,

                                                  BOTTOM_ROW,

                              TO_SYMB, TO_ALT, TO_SHCT,   TO_NAV, TO_CTL, TO_SFT
    ),

    [LR_CTRL] = ML_LAYOUT(
        CL(EQL),  CL(1),   CL(2),  CL(3),   CL(4),   CL(5),   RCTL(WS_LEFT),  CK_ESC,  CL(6),   CL(7),   CL(8),   CL(9),    CL(0),    TO_SFTY,
        CL(TAB),  CL(Q),   CTRL_W, KC_END,  CL(R),   CL(T),   KC_LEFT,        KC_RGHT, CL(Y),   CK_TABB, CL(I),   CK_TABF,  KC_UP,    CL(BSLS),
LSFT_T(CL(LPRN)), KC_HOME, CL(F),  KC_DEL,  KC_RGHT, CK_CTLG, _______,        _______, KC_BSPC, TGL_SHF, CK_KILL, KC_PGUP,  CL(SCLN), RSFT_T(CL(RPRN)),
        CL(LCBR), CL(Z),   CTRL_X, CK_COPY, KC_PGDN, KC_LEFT,                                   KC_DOWN, CL(M),   CL(COMM), CL(DOT),  CL(SLSH), CL(RCBR),

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
        AL(TAB),  AL(Q), AL(F4), AL(E),   AL(R),     CK_ALTT, KC_LEFT,              KC_RIGHT,      CK_PSTE, TGL_SLT, AL(I),    TGL_ALT, AL(P),    AL(BSLS),
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
        _______,  RESET,   CK_WWWB, MS_MID,  CK_WWWF, _______, _______,           _______, URL_PST, URL_COPY, URL_ICP, CK_MOMA, CK_CL,   _______,
        KC_ENTER, CL(A),   MS_LEFT, MS_SMID, MS_RGHT, _______, _______,           _______, _______, GD_HD_4,  GD_HD_5, _______, _______, _______,
        _______,  CK_MCR2, CK_MCR1, MS_CTRL, _______, GD_BULT,                             CK_NEW,  GD_HD_1,  GD_HD_2, GD_HD_3, _______, _______,

                                                                     BOTTOM_ROW,

                                           _______, _______, _______,           _______, _______, _______
    ),

    [LR_NAVIGATION] = ML_LAYOUT(
        _______, _______, _______, _______, _______,  _______,     _______,           _______, _______,     _______, _______, _______, _______, TO_SFTY,
        _______, _______, CK_WWWB, _______, RESET,    RCTL(AL(T)), _______,           _______, CL(T),       CK_TABB, CL(W),   CK_TABF, GU(UP),  TGL_ALT,
        _______, _______, _______, _______, GU(RGHT), _______,     _______,           _______, RCTL(SH(T)), WS_LEFT, CODE_WS, WS_RGHT, MISC_WS, TGL_ELT,
        _______, _______, _______, _______, _______,  GU(LEFT),                                GU(DOWN),    _______, WS_UP,   WS_DOWN, _______, _______,

                                                                      BOTTOM_ROW,

                                            GU(TAB), _______, _______,           _______, _______, _______
    ),

    [LR_SYMB] = ML_LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,           _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  TO_SFTY,
        _______, KC_EXLM, KC_COLN, KC_EQL,  KC_CIRC, KC_LPRN, _______,           _______, KC_RPRN, KC_7,    KC_8,    KC_9,    KC_PERC, KC_F12,
        _______, KC_AMPR, KC_ASTR, KC_DLR,  KC_SLSH, KC_LBRC,  _______,          _______, CK_MDPS, KC_4,    KC_5,    KC_6,    KC_0,    KC_PERC,
        _______, KC_TILD, KC_EXLM, KC_AT,   KC_EQL,  KC_BSLS,                             KC_RCBR, KC_1,    KC_2,    KC_3,    KC_0,    _______,
                                                           BOTTOM_ROW,
                                            RGB_HUD, KC_ENTER, RGB_HUI, TOGGLE_LAYER_COLOR, KC_SPACE, _______
    ),

    [LR_OUTLOOK] = ML_LAYOUT(
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______,   _______, _______, TO_SFTY,
        _______, _______, _______, _______, _______, _______, _______,           _______, CL(U),   CL(Q),   CL(COMMA), CK_OLRL, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, CL(1),   CL(DOT),   CL(2),   _______, _______,
        _______, _______, _______, _______, _______, _______,                             _______, CL(U),   _______,   CL(M),   _______, _______,

                                                           BOTTOM_ROW,

                                            _______, _______, _______,           _______, _______, _______
    ),

    /*
    [EMPTY_LAYER] = ML_LAYOUT(
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______, TO_SFTY,
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                             _______, _______, _______, _______, _______, _______,

        _______, _______, _______, _______, _______,          _______,           _______,          _______, _______, _______, _______, _______,

                                            _______, _______, _______,           _______, _______, _______
    ),
    */
};
