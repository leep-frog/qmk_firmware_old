#ifndef LEEP_INTERFACE
#define LEEP_INTERFACE

// Outline interface that each keyboard must implement

#ifndef LEEP_SAFE_RANGE
#error Must define LEEP_SAFE_RANGE in keymap.c
#endif

// Run when reset key is hit (but before actual keyboard reset)
void on_reset(void);

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
