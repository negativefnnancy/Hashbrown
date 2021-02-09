#ifndef UI_BOX_STYLE_H
#define UI_BOX_STYLE_H

#include "../color.h"

typedef struct ui_box_style_t {

    double size_margin;
    double size_border;
    double size_padding;
    color_t color_margin;
    color_t color_border;
    color_t color_padding;

} ui_box_style_t;

ui_box_style_t ui_box_style_make (double size_margin,
                                  double size_border,
                                  double size_padding,
                                  color_t color_margin,
                                  color_t color_border,
                                  color_t color_padding);

#endif /* UI_BOX_STYLE_H */
