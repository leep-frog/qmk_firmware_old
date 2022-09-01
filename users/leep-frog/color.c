#ifndef LEEP_COLOR
#define LEEP_COLOR

#define ENABLE_LEEP_COLOR

#ifdef ENABLE_LEEP_COLOR

// This macro is incase we ever switch from hsv representation to rgb or vice versa.
#define LEEP_C(clr) RGB_ ## clr

#define LEEP_LAYER_COLOR(lyr) rgb_matrix_set_color_all(layer_colors[lyr][0], layer_colors[lyr][1], layer_colors[lyr][2])
#define LEEP_SYS_COLOR(clr) rgb_matrix_set_color_all(LEEP_C(clr))

int layer_colors[NUM_LAYERS][3] = {
  [0 ... NUM_LAYERS - 1] = { LEEP_C(GREEN) },
  [LR_BASE] = { LEEP_C(CYAN) },
  [LR_SAFE] = { LEEP_C(GREEN) },
  [LR_CTRL] = { LEEP_C(SPRINGGREEN) },
  [LR_ALT] = { LEEP_C(MAGENTA) },
  [LR_CTRL_X] = { LEEP_C(CHARTREUSE) },
  [LR_CTRL_ALT] = { LEEP_C(GOLD) },
  [LR_NAVIGATION] = { LEEP_C(CORAL) },
  [LR_SHORTCUTS] = { LEEP_C(TURQUOISE) },
  [LR_SYMB] = { LEEP_C(ORANGE) },
  [LR_OUTLOOK] = { LEEP_C(BLUE) },
  // We don't change the keyboard color for shift because there isn't
  // a way to only do it on mod and not on tap as well, and seeing the
  // color change for every space character was really annoying.
};

#else // ifdef ENABLE_LEEP_COLOR

#define LEEP_LAYER_COLOR(lyr)

#endif // ifdef ENABLE_LEEP_COLOR

#endif // ifndef LEEP_COLOR
