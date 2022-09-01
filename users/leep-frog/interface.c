#ifndef LEEP_INTERFACE
#define LEEP_INTERFACE

// Outline interface that each keyboard must implement

#ifndef LEEP_SAFE_RANGE
#error Must define LEEP_SAFE_RANGE in keymap.c
#endif

// Run when reset key is hit (but before actual keyboard reset).
// Should return whether or not to actually reset the keyboard.
void on_reset(uint8_t count);

// Run when muted with mute key 1 (audio signal)
void on_mute_1(void);

// Run when unmuted with mute key 1 (audio signal)
void on_unmute_1(void);

// Run when muted with mute key 2 (non-audio signal)
void on_mute_2(void);

// Run when unmuted with mute key 2 (non-audio signal)
void on_unmute_2(void);

// Run when shift is toggled
void on_shift(void);

// Run when shift is untoggled
void on_unshift(void);

// Run when recording starts
void recording_start(bool macro_1);

// Run when recording ends
void recording_end(bool macro_1);

// Run when recording plays
void recording_play(bool macro_1);

// Run whenever there is a layer change, the provided argument
// being the highest active layer.
void on_layer_change(uint8_t layer);

#endif
