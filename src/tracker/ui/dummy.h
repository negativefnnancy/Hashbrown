#ifndef UI_DUMMY_H
#define UI_DUMMY_H

#include "box.h"
#include "../color.h"

typedef struct ui_dummy_t {

    ui_box_t base;
    color_t color;

} ui_dummy_t;

void ui_dummy_init (ui_dummy_t *element, color_t color, ui_box_style_t style);

void ui_dummy_draw (ui_dummy_t *element,
                    struct interface_t *interface,
                    region_t clipping_region,
                    region_t element_region);

ui_dummy_t *ui_dummy_create  (color_t color, ui_box_style_t style);
void        ui_dummy_destroy (ui_dummy_t *element);

#endif /* UI_DUMMY_H */
