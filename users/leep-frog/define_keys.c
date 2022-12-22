#ifndef LEEP_DK
#define LEEP_DK

// TODO: enums should be CK_*, defined should be DK_*

#define CTRL_W RCTL(KC_W)

#define CK_UNDO CL(Z)

// Note: we use page down and page up because that works with chrome
// and "tab" is a special character in some terminals and sometimes gets
// modified or ignored when passed to processes inside of the terminal.
#define CK_TABF RCTL(KC_PGDOWN)  // Next tab in chrome
#define CK_TABB LCTL(KC_PGUP)    // Previous tab in chrome
#define CK_WWWF LALT(KC_RIGHT)   // Next page in chrome
#define CK_WWWB LALT(KC_LEFT)    // Previous page in chrome

// Mouse movements
#define MS_LEFT KC_MS_BTN1
#define MS_RGHT KC_MS_BTN2
#define MS_MID KC_MS_BTN3
#define MS_SMID RSFT(KC_MS_BTN3)

#define CK_NEW RCTL(RSFT(KC_N))

// Windows snip (screenshot)
#define CK_SNIP RGUI(RSFT(KC_S))

// Copy with ctrl+c (instead of ctrl+<insert>) so ctrl+c functionality
// still works (like cancelling in bash terminal).
#define CK_COPY RCTL(KC_C)
#define CK_PSTE RSFT(KC_INSERT)

// These use IFTTT and use custom keyboard shortcuts under the hood.
#define LGHT_ON RCTL(RALT(RSFT(KC_8)))
#define LGHT_OF RCTL(RALT(RSFT(KC_9)))

#define SS_PASTE SS_RSFT(SS_TAP(X_INSERT))
#define SS_COPY SS_RCTL(SS_TAP(X_INSERT))

#endif
