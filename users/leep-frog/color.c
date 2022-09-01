#ifndef LEEP_COLOR
#define LEEP_COLOR

#define HSV_RED_ORANGE 9, 255, 255

//#define LEEP_LAYER_COLOR(lyr) rgb_matrix_set_color_all(layer_colors[lyr][0], layer_colors[lyr][1], layer_colors[lyr][2])
#define ENABLE_LEEP_COLOR

#ifdef ENABLE_LEEP_COLOR
#define LEEP_LAYER_COLOR(lyr) rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR); rgblight_sethsv(layer_colors[lyr][0], layer_colors[lyr][1], layer_colors[lyr][2])
#define LEEP_SOLID_COLOR(clr) rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR); rgblight_sethsv(clr)
//#define LEEP_COLOR_MODE(clr, mde) rgb_matrix_mode(mde); rgblight_sethsv(clr)
#define LEEP_COLOR_MODE(clr, mde) rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR); rgblight_sethsv(clr)

// Color on shift
int layer_colors[NUM_LAYERS][3] = {
  [0 ... NUM_LAYERS - 1] = { HSV_GREEN },
  [LR_BASE] = { HSV_CYAN },
  [LR_SAFE] = { HSV_GREEN },
  [LR_CTRL] = { HSV_PURPLE },
  [LR_ALT] = { HSV_BLUE },
  [LR_CTRL_X] = { HSV_CHARTREUSE },
  [LR_CTRL_ALT] = { HSV_GOLD },
  [LR_NAVIGATION] = { HSV_ORANGE },
  [LR_SHORTCUTS] = { HSV_RED_ORANGE },
  [LR_SYMB] = { HSV_MAGENTA },
  [LR_OUTLOOK] = { HSV_SPRINGGREEN },
  // We don't change the keyboard color for shift because there isn't
  // a way to only do it on mod and not on tap as well, and seeing the
  // color change for every space character was really annoying.
};

#else // ifdef ENABLE_LEEP_COLOR

#define LEEP_LAYER_COLOR(lyr)
#define LEEP_SOLID_COLOR(clr)
#define LEEP_COLOR_MODE(clr, mde)

#endif // ifdef ENABLE_LEEP_COLOR

#endif // ifndef LEEP_COLOR
