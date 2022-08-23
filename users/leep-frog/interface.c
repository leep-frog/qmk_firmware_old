#ifndef LEEP_INTERFACE
#define LEEP_INTERFACE

// Outline interface that each keyboard must implement

#ifndef LEEP_SAFE_RANGE
#error Must define LEEP_SAFE_RANGE in keymap.c
#endif

// Run when recording starts
void recording_start(void);

// Run when recording ends
void recording_end(void);

// Run whenever there is a layer change, the provided argument
// being the highest active layer.
void on_layer_change(uint8_t layer);

#endif
