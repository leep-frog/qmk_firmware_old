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

enum custom_stringers { // prefix 'CS_'
  CS_ENUM_START = LEEP_SAFE_RANGE,

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

  CS_ENUM_END,
};

#define NUM_CS CS_ENUM_END - CS_ENUM_START - 1

enum custom_url_strings { // prefix 'CU_'
  // TODO: These start and end ones are making us waste values. Fix this.
  CU_ENUM_START = CS_ENUM_END,

  // Copy URL from a Chrome browser
  URL_COPY,
  // Copy URL ID
  URL_ICP,

  CU_ENUM_END,
};

#define NUM_CU CU_ENUM_END - CU_ENUM_START - 1

enum custom_new_tab_stringers { // prefix 'CN_'
  CN_ENUM_START = CU_ENUM_END,

  // Paste URL
  URL_PST,

  CK_CL,
  CK_MOMA,

  CN_ENUM_END,
};

#define NUM_CN CN_ENUM_END - CN_ENUM_START - 1

enum custom_keycodes { // prefix CK (custom keycode)
  CK_ENUM_START = CN_ENUM_END,

  CK_CTLG,
  //MS_CTRL,
  CK_ALTT,
  CK_MUT1,
  CK_MUT2,

  CK_ENUM_END,
};

#define NUM_CK CK_ENUM_END - CK_ENUM_START - 1

#define PROCESSOR

enum td_keys { // prefix TDK
    TDK_SHIFT_TOGGLE,
    TDK_KILL_LINE,
    TDK_MACRO_1,
    TDK_MACRO_2,
    TDK_BACKSPACE,
    TDK_DELETE,
    TDK_MARKDOWN_PASTE,
    TDK_OUTLOOK_RELOAD,
};

#endif
