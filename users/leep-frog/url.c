#ifndef LEEP_HELPER
#define LEEP_HELPER

// URLWait waits a short amount of time to allow the
// internet browser to run actions (like opening a new tab).
void URLWait(void) {
    wait_ms(50);
}

#define NEW_TAB() SEND_STRING(SS_RCTL("t")); URLWait()

#define URL_COPY() SEND_STRING(SS_DOWN(X_RCTL) "l" ); URLWait(); SEND_STRING("c" SS_UP(X_RCTL))
#define URL_PASTE() NEW_TAB(); SEND_STRING(SS_PASTE SS_TAP(X_ENTER));

#endif
