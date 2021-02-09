#ifndef UI_BOX_H
#define UI_BOX_H

#include "element.h"

struct ui_box_t;

typedef void ui_box_draw_method_t (struct ui_box_t *element,
                                   struct interface_t *interface,
                                   region_t clipping_region,
                                   region_t element_region);

typedef struct ui_box_t {

    ui_element_t base;
    ui_box_draw_method_t *method_draw;

} ui_box_t;

void ui_box_init (ui_box_t *element,
                  ui_box_draw_method_t *method_draw);

ui_box_draw_method_t ui_box_draw;

#endif /* UI_BOX_H */
