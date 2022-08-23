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
    VRSN = LEEP_SAFE_RANGE,
    NUM_CK_START,
    // Toggle alt tab
    TGL_ALT,
    // Toggle shift-alt tab
    TGL_SLT,
    // End alt tab
    TGL_ELT,
    // Toggle shift
    TGL_SHF,
    CK_CL,
    CK_MOMA,
    URL_COPY,
    // Copy URL ID
    URL_ICP,
    // URL_PST
    URL_PST,
    CK_NEW,
    CK_CTLG,
    CK_ESC,
    CK_UNBS,
    MS_CTRL,
    CK_ALTT,

    NUM_CK_END,
};

#define PROCESSOR

bool k = true;

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
