#include "box_style.h"

ui_box_style_t ui_box_style_make (double size_margin,
                                  double size_border,
                                  double size_padding,
                                  color_t color_margin,
                                  color_t color_border,
                                  color_t color_padding) {

    ui_box_style_t style;
    style.size_margin   = size_margin;
    style.size_border   = size_border;
    style.size_padding  = size_padding;
    style.color_margin  = color_margin;
    style.color_border  = color_border;
    style.color_padding = color_padding;
    return style;
}
