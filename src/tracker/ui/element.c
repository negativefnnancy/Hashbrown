#include "element.h"
#include "../interface.h"

void ui_element_init (ui_element_t *element,
                      ui_element_draw_method_t *method_draw,
                      ui_element_event_method_t *method_event) {

    element->method_draw  = method_draw;
    element->method_event = method_event;
}

void ui_element_draw (ui_element_t *element,
                      interface_t *interface,
                      region_t clipping_region,
                      region_t element_region) {

    element->method_draw (element, interface, clipping_region, element_region);
}

void ui_element_event (ui_element_t *element,
                       interface_t *interface,
                       ui_event_t event,
                       region_t element_region) {

    element->method_event (element, interface, event, element_region);
}
