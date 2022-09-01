#ifndef LEEP_COLOR
#define LEEP_COLOR

#define ENABLE_LEEP_COLOR

#ifdef ENABLE_LEEP_COLOR

#define LEEP_LAYER_COLOR(lyr) rgb_matrix_set_color_all(layer_colors[lyr][0], layer_colors[lyr][1], layer_colors[lyr][2])
#define LEEP_SYS_COLOR(clr) rgb_matrix_set_color_all(clr)

int layer_colors[NUM_LAYERS][3] = {
  [0 ... NUM_LAYERS - 1] = { RGB_GREEN },
  [LR_BASE] = { RGB_CYAN },
  [LR_SAFE] = { RGB_GREEN },
  [LR_CTRL] = { RGB_SPRINGGREEN },
  [LR_ALT] = { RGB_MAGENTA },
  [LR_CTRL_X] = { RGB_CHARTREUSE },
  [LR_CTRL_ALT] = { RGB_GOLD },
  [LR_NAVIGATION] = { RGB_CORAL },
  [LR_SHORTCUTS] = { RGB_TURQUOISE },
  [LR_SYMB] = { RGB_ORANGE },
  [LR_OUTLOOK] = { RGB_BLUE },
  // We don't change the keyboard color for shift because there isn't
  // a way to only do it on mod and not on tap as well, and seeing the
  // color change for every space character was really annoying.
};

#else // ifdef ENABLE_LEEP_COLOR

#define LEEP_LAYER_COLOR(lyr)

#endif // ifdef ENABLE_LEEP_COLOR

#endif // ifndef LEEP_COLOR
