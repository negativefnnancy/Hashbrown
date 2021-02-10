#ifndef UI_BOX_H
#define UI_BOX_H

#include "element.h"
#include "box_style.h"

struct ui_box_t;

typedef void ui_box_draw_method_t (struct ui_box_t *element,
                                   struct interface_t *interface,
                                   region_t clipping_region,
                                   region_t element_region);

typedef void ui_box_event_method_t (struct ui_box_t *element,
                                    struct interface_t *interface,
                                    event_t event,
                                    region_t element_region);

typedef struct ui_box_t {

    ui_element_t base;
    ui_box_draw_method_t  *method_draw;
    ui_box_event_method_t *method_event;
    ui_box_style_t style;

} ui_box_t;

void ui_box_init (ui_box_t *element,
                  ui_box_draw_method_t *method_draw,
                  ui_box_event_method_t *method_event,
                  ui_box_style_t style);

ui_box_draw_method_t  ui_box_draw;
ui_box_event_method_t ui_box_event;

#endif /* UI_BOX_H */
