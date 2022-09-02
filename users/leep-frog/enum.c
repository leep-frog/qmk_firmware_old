#ifndef LEEP_ENUMS
#define LEEP_ENUMS

enum layers { // prefix LR
    LR_BASE, // default layeqr
    LR_CTRL,
    LR_ALT,
    LR_SYMB, // symbols
    LR_CTRL_X, // layer activated temporarily after hitting x in the CTRL layer.
    LR_SHORTCUTS, // command for combination keystrokes.
    LR_NAVIGATION, // command for changing workspaces.
    LR_SAFE,
    LR_OUTLOOK,
    LR_CTRL_ALT,

    NUM_LAYERS,
};

// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_rgblight.md#defining-lighting-layers-iddefining-lighting-layers
#ifdef RGBLIGHT_MAX_LAYERS
#undef RGBLIGHT_MAX_LAYERS
#endif
#define RGBLIGHT_MAX_LAYERS NUM_LAYERS

enum td_keys { // prefix TDK
    TDK_SHIFT_TOGGLE,
    TDK_KILL_LINE,
    TDK_MACRO_1,
    TDK_MACRO_2,
    TDK_BACKSPACE,
    TDK_DELETE,
    TDK_MARKDOWN_PASTE,
    TDK_OUTLOOK_RELOAD,
    TDK_RESET,
};

// Custom keycode enums
#define LEEP_ENUM(name, prefix, start, ...) enum name {\
  prefix ## _ENUM_START = start - 1,\
  __VA_ARGS__\
  prefix ## _ENUM_END,\
};

#define LEEP_ENUM_OFFSET(prefix, v) v - prefix ## _ENUM_START - 1

#define LEEP_ENUM_CASE(prefix) prefix ## _ENUM_START + 1 ... prefix ## _ENUM_END - 1

LEEP_ENUM(custom_strings, CS, LEEP_SAFE_RANGE + 1,
  // Toggle alt tab
  TGL_ALT,
  // Toggle shift-alt tab
  TGL_SLT,
  // End alt tab
  TGL_ELT,

  // Universal backspace
  CK_UNBS,
  // Escape
  CK_ESC,
)

LEEP_ENUM(custom_url_strings, CU, CS_ENUM_END,
  // Copy URL from a Chrome browser
  URL_COPY,
  // Copy URL ID
  URL_ICP,
)

LEEP_ENUM(custom_new_tab_strings, CN, CU_ENUM_END,
  // Paste URL
  URL_PST,

  CK_CL,
  CK_MOMA,
)

LEEP_ENUM(custom_keycodes, CK, CN_ENUM_END,
  CK_CTLG,
  CK_ALTT,
  CK_MUT1,
  CK_MUT2,
  MS_CTRL,
  CK_EYE,
)

#endif
