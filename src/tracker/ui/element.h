#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "event.h"
#include "../region.h"

struct ui_element_t;
struct interface_t;

typedef void ui_element_draw_method_t (struct ui_element_t *element,
                                       struct interface_t *interface,
                                       region_t clipping_region,
                                       region_t element_region);

typedef void ui_element_event_method_t (struct ui_element_t *element,
                                        struct interface_t *interface,
                                        ui_event_t event,
                                        region_t element_region);

typedef struct ui_element_t {

    ui_element_draw_method_t  *method_draw;
    ui_element_event_method_t *method_event;

} ui_element_t;

void ui_element_init (ui_element_t *element,
                      ui_element_draw_method_t *method_draw,
                      ui_element_event_method_t *method_event);

ui_element_draw_method_t  ui_element_draw;
ui_element_event_method_t ui_element_event;

#endif /* UI_ELEMENT_H */
