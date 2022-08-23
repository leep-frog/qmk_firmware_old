#include QMK_KEYBOARD_H
#include "version.h"

#define LEEP_SAFE_RANGE ML_SAFE_RANGE

#include "../../../../users/leep-frog/main.c"

// Color on shift
int layer_colors[NUM_LAYERS][3] = {
  [LR_BASE] = { RGB_CYAN },
  [LR_SAFE] = { RGB_GREEN },
  [LR_CTRL] = { RGB_ORANGE },
  [LR_ALT] = { RGB_MAGENTA },
  [LR_CTRL_X] = { RGB_YELLOW },
  [LR_CTRL_ALT] = { RGB_GOLD },
  [LR_NAVIGATION] = { RGB_PINK },
  [LR_SHORTCUTS] = { RGB_CORAL },
  [LR_SYMB] = { RGB_PURPLE },
  [LR_OUTLOOK] = { RGB_BLUE },
};

// Interface functions
void on_layer_change(uint8_t layer) {
  if (!recording) {
    rgb_matrix_set_color_all(layer_colors[layer][0], layer_colors[layer][1], layer_colors[layer][2]);
  }
}

// See below link for more sounds. Also pretty easy to make sounds if need be
// Do the link sound!
// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_audio.md#songs
#define DEFINE_SONG(var_name, sound) float var_name[][2] = SONG(sound);
// Note, some of these are declared elsewhere, but was hopping between duplicate
// definition and undefined errors. I assume the music_on_song is declared
// after this file is processed.
DEFINE_SONG(song_music_on, MUSIC_ON_SOUND)
DEFINE_SONG(song_music_off, MUSIC_ON_SOUND)
DEFINE_SONG(song_startup, STARTUP_SOUND)
DEFINE_SONG(song_goodbye, GOODBYE_SOUND)
DEFINE_SONG(song_qwerty, QWERTY_SOUND)

// https://www.ninsheetmusic.org/download/pdf/3544
#define LINK_SOUND H__NOTE(_G7), H__NOTE(_F7), H__NOTE(_DS7), H__NOTE(_A6), H__NOTE(_GS6), H__NOTE(_E7), H__NOTE(_GS7), H__NOTE(_C8)
#define ITEM_OBTAINED H__NOTE(_FS6), H__NOTE(_AS6), H__NOTE(_CS7), H__NOTE(_B6), H__NOTE(_DS7), H__NOTE(_FS7), H__NOTE(_CS8)

DEFINE_SONG(song_link, LINK_SOUND)
DEFINE_SONG(song_item, ITEM_OBTAINED)

void recording_start(void) {
  // https://github.com/qmk/qmk_firmware/blob/master/docs/feature_audio.md
  PLAY_SONG(song_link);
  rgb_matrix_set_color_all(RGB_RED);
  rgb_matrix_mode(RGB_MATRIX_BREATHING);
}

void recording_end(void) {
  PLAY_SONG(song_item);
  on_layer_change(get_highest_layer(layer_state));
}

// Can't evaluate macro in macro, so use this to ignore bottom row of keyboard
// https://stackoverflow.com/questions/35114050/is-there-a-way-to-force-c-preprocessor-to-evaluate-macro-arguments-before-the-ma
#define ML_LAYOUT(...) LAYOUT_moonlander(__VA_ARGS__)
#define EMPTY_BOTTOM_ROW _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LR_BASE] = ML_LAYOUT(
        CK_LOCK, KC_1, KC_2, KC_3, KC_4, KC_5, WS_LEFT,       CK_ESC,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    TO_SFTY,
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, LGHT_ON,       LGHT_OF, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
        KC_LSPO, KC_A, KC_S, KC_D, KC_F, KC_G, WS_LEFT,       WS_RGHT, KC_H, KC_J, KC_K,    KC_L,   TO_OTLK, KC_RSPC,
        KC_LCBR, KC_Z, KC_X, KC_C, KC_V, KC_B,                         KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCBR,
                                                   EMPTY_BOTTOM_ROW,

                              TO_SHCT, TO_SYMB, TO_ALT,  TO_CTL, TO_SFT, TO_NAV
    ),

    [LR_SAFE] = ML_LAYOUT(
        CK_LOCK, KC_1, KC_2, KC_3, KC_4, KC_5, _______,       CK_ESC,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    TO(LR_BASE),
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, LGHT_ON,       LGHT_OF, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
        KC_LSPO, KC_A, KC_S, KC_D, KC_F, KC_G, WS_LEFT,       WS_RGHT, KC_H, KC_J, KC_K,    KC_L,   TO_OTLK, KC_RSPC,
        KC_LCBR, KC_Z, KC_X, KC_C, KC_V, KC_B,                         KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCBR,

                                                  EMPTY_BOTTOM_ROW,

                              TO_SHCT, TO_SYMB, TO_ALT,       TO_CTL, TO_SFT, TO_NAV
    ),

    [LR_CTRL] = ML_LAYOUT(
        CL(EQL),  CL(1),   CL(2),  CL(3),   CL(4),   CL(5),   RCTL(WS_LEFT),  CK_ESC,  CL(6),   CL(7),   CL(8),   CL(9),    CL(0),    TO_SFTY,
        CL(TAB),  CL(Q),   CTRL_W, KC_END,  CL(R),   CL(T),   KC_LEFT,        KC_RGHT, CL(Y),   CK_TABB, CL(I),   CK_TABF,  KC_UP,    CL(BSLS),
LSFT_T(CL(LPRN)), KC_HOME, CL(F),  KC_DEL,  KC_RGHT, CK_CTLG, _______,        _______, KC_BSPC, TGL_SHF, CK_KILL, KC_PGUP,  CL(SCLN), RSFT_T(CL(RPRN)),
        CL(LCBR), CL(Z),   CTRL_X, CK_COPY, KC_PGDN, KC_LEFT,                                   KC_DOWN, CL(M),   CL(COMM), CL(DOT),  CL(SLSH), CL(RCBR),

                                                           EMPTY_BOTTOM_ROW,

                            CL(TAB), CL(ENTER), MO(LR_CTRL_ALT),    _______, CL(SPACE), RGUI_T(KC_RCBR)
    ),

    [LR_CTRL_X] = ML_LAYOUT(
        _______,  CL(1), CL(2), CL(3), CL(4), CL(5), RCTL(WS_LEFT),           RCTL(CK_ESC),      CL(6), CL(7), CL(8),     CL(9),    CL(0),    TO_SFTY,
        CL(TAB),  CL(Q), CL(W), CL(E), CL(R), CL(T), KC_LEFT,                 KC_RIGHT, CK_PSTE, CL(U), CL(I), CL(O),     CL(P),    CL(BSLS),
        CL(LSPO), CL(A), CL(S), CL(D), CL(F), CL(G), RCTL(WS_LEFT),           RCTL(WS_RGHT),     CL(H), CL(J), CL(K),     CL(L),    CL(SCLN), CL(RSPC),
        CL(LCBR), CL(Z), CL(X), CL(C), CL(V), CL(B),                                             CL(N), CL(M), CL(COMMA), CL(DOT),  CL(SLSH), CL(RCBR),

                                                           EMPTY_BOTTOM_ROW,

                                CL(TAB), TO_SYMB, CL(DEL),        CL(BSPC), TO_SFT, _______
    ),

    [LR_ALT] = ML_LAYOUT(
        AL(EQL),  AL(1), AL(2),  AL(3),   AL(4),     AL(5),   RALT(WS_LEFT),        RALT(CK_ESC),  AL(6),   AL(7),   AL(8),    AL(9),   AL(0),    TO_SFTY,
        AL(TAB),  AL(Q), AL(F4), AL(E),   AL(R),     CK_ALTT, KC_LEFT,              KC_RIGHT,      CK_PSTE, TGL_SLT, AL(I),    TGL_ALT, AL(P),    AL(BSLS),
LSFT_T(AL(LPRN)), AL(A), AL(S),  CL(DEL), CL(RIGHT), AL(G),   RALT(WS_LEFT),        RALT(WS_RGHT), CK_UNBS, AL(J),   AL(K),    KC_WH_U, AL(SCLN), RSFT_T(AL(RPRN)),
        AL(LCBR), AL(Z), AL(X),  AL(C),   KC_WH_D,   CL(LEFT),                                     AL(N),   AL(M),   AL(COMM), AL(DOT), AL(SLSH), AL(RCBR),

                                                                     EMPTY_BOTTOM_ROW,

                                                  AL(TAB), AL(ENTER), _______,       MO(LR_CTRL_ALT), AL(SPACE), RGUI_T(KC_RCBR)
    ),

    // This layer is basically identical to the safe layer aside from the thumb keys.
    [LR_CTRL_ALT] = ML_LAYOUT(
        CK_LOCK, KC_1, KC_2, KC_3, KC_4, KC_5, _______,       CK_ESC,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    TO_SFTY,
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T, LGHT_ON,       LGHT_OF, KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
        KC_LSPO, KC_A, KC_S, KC_D, KC_F, KC_G, WS_LEFT,       WS_RGHT, KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_RSPC,
        KC_LCBR, KC_Z, KC_X, KC_C, KC_V, KC_B,                         KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCBR,

                                                EMPTY_BOTTOM_ROW,

                             KC_TAB, KC_ENTER, _______,        _______, KC_SPACE, KC_RGUI
    ),

   [LR_SHORTCUTS] = ML_LAYOUT(
        _______,  _______, _______, _______, _______, _______, _______,           _______, _______, _______,  _______, _______, _______, TO_SFTY,
        _______,  RESET,   CK_WWWB, MS_MID,  CK_WWWF, _______, _______,           _______, URL_PST, URL_COPY, URL_ICP, CK_MOMA, CK_CL,   _______,
        KC_ENTER, CL(A),   MS_LEFT, MS_SMID, MS_RGHT, _______, _______,           _______, _______, GD_HD_4,  GD_HD_5, _______, _______, _______,
        _______,  CK_MCR2, CK_MCR1, MS_CTRL, _______, GD_BULT,                             CK_NEW,  GD_HD_1,  GD_HD_2, GD_HD_3, _______, _______,

                                                                     EMPTY_BOTTOM_ROW,

                                           _______, _______, _______,           _______, TGL_SHF, _______
    ),

    [LR_NAVIGATION] = ML_LAYOUT(
        _______, _______, _______, _______, _______,  _______,     _______,           _______, _______,     _______, _______, _______, _______, TO_SFTY,
        _______, _______, CK_WWWB, _______, RESET,    RCTL(AL(T)), _______,           _______, CL(T),       CK_TABB, CL(W),   CK_TABF, GU(UP),  TGL_ALT,
        _______, _______, _______, _______, GU(RGHT), _______,     _______,           _______, RCTL(SH(T)), WS_LEFT, CODE_WS, WS_RGHT, MISC_WS, TGL_ELT,
        _______, _______, _______, _______, _______,  GU(LEFT),                                GU(DOWN),    _______, WS_UP,   WS_DOWN, _______, _______,

                                                                      EMPTY_BOTTOM_ROW,

                                            GU(TAB), _______, _______,           _______, _______, _______
    ),

    [LR_SYMB] = ML_LAYOUT(
        VRSN,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,           _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  TO_SFTY,
        _______, KC_EXLM, KC_COLN, KC_EQL,  KC_CIRC, KC_LPRN, _______,           _______, KC_RPRN, KC_7,    KC_8,    KC_9,    KC_PERC, KC_F12,
        _______, KC_AMPR, KC_ASTR, KC_DLR,  KC_SLSH, KC_LBRC,  _______,          _______, KC_DOWN, KC_4,    KC_5,    KC_6,    KC_0,    KC_PERC,
        _______, KC_TILD, KC_EXLM, KC_AT,   KC_EQL,  KC_LCBR,                             KC_RCBR, KC_1,    KC_2,    KC_3,    KC_0,    _______,
                                                           EMPTY_BOTTOM_ROW,
                                            RGB_HUD, KC_ENTER, RGB_HUI, TOGGLE_LAYER_COLOR, KC_SPACE, _______
    ),

    [LR_OUTLOOK] = ML_LAYOUT(
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, _______, _______,   _______, _______, TO_SFTY,
        _______, _______, _______, _______, _______, _______, _______,           _______, CL(U),   CL(Q),   CL(COMMA), CK_OLRL, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,           _______, _______, CL(1),   CL(DOT),   CL(2),   _______, _______,
        _______, _______, _______, _______, _______, _______,                             _______, CL(U),   _______,   CL(M),   _______, _______,

                                                           EMPTY_BOTTOM_ROW,

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
