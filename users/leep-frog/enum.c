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

enum press_type { // prefix PT
    PT_PRESS,
    PT_UNPRESS,
};

enum custom_keycodes { // prefix CK (custom keycode)
  CK_ENUM_START = LEEP_SAFE_RANGE,

  /***************
   * Toggle keys *
   ***************/
  // Toggle alt tab
  TGL_ALT,
  // Toggle shift-alt tab
  TGL_SLT,
  // End alt tab
  TGL_ELT,

  /************
   * URL keys *
   ************/
  // Copy URL from a Chrome browser
  URL_COPY,
  // Copy URL ID
  URL_ICP,
  // Paste URL
  URL_PST,

  /***************
   * Custom keys *
   ***************/
  CK_CTLG,
  CK_ESC,
  CK_UNBS,
  MS_CTRL,
  CK_ALTT,
  CK_MUT1,
  CK_MUT2,
  CK_CL,
  CK_MOMA,

  CK_ENUM_END,
};

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
