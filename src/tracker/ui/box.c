#include "box.h"
#include "../interface.h"
#include "../util.h"

void ui_element_draw_method_box (ui_element_t *element,
                                 interface_t *interface,
                                 region_t clipping_region,
                                 region_t element_region) {

    ui_box_draw ((ui_box_t *) element,
                 interface,
                 clipping_region,
                 element_region);
}

void ui_element_event_method_box (ui_element_t *element,
                                  interface_t *interface,
                                  ui_event_t event,
                                  region_t element_region) {

    ui_box_event ((ui_box_t *) element,
                  interface,
                  event,
                  element_region);
}

void ui_box_init (ui_box_t *element,
                  ui_box_draw_method_t *method_draw,
                  ui_box_event_method_t *method_event,
                  ui_box_style_t style) {

    ui_element_init ((ui_element_t *) element,
                     ui_element_draw_method_box,
                     ui_element_event_method_box);
    element->method_draw  = method_draw;
    element->method_event = method_event;
    element->style = style;
}

/* TODO rename to region_clipping and region_element */

void ui_box_draw (ui_box_t *element,
                  interface_t *interface,
                  region_t clipping_region,
                  region_t element_region) {

    region_t margin_region, border_region, padding_region, content_region;
    region_t intersection;

    /* calculate the inner regions */
    margin_region  = element_region;
    border_region  = region_inset (element_region, element->style.size_margin);
    padding_region = region_inset (border_region,  element->style.size_border);
    content_region = region_inset (padding_region, element->style.size_padding);

    /* draw the margin */
    draw_box (interface->renderer, clipping_region, margin_region, border_region, element->style.color_margin);

    /* draw the border */
    draw_box (interface->renderer, clipping_region, border_region, padding_region, element->style.color_border);

    /* draw the padding */
    draw_box (interface->renderer, clipping_region, padding_region, content_region, element->style.color_padding);

    /* draw the content */
    intersection = region_intersection (content_region, clipping_region);
    element->method_draw (element, interface, intersection, content_region);
}

void ui_box_event (ui_box_t *element,
                   interface_t *interface,
                   ui_event_t event,
                   region_t element_region) {

    double offset;
    region_t content_region;

    /* calculate the inner region */
    offset = element->style.size_margin
           + element->style.size_border
           + element->style.size_padding;
    content_region = region_inset (element_region, offset);

    /* TODO offset the event position */

    /* handle the event the content */
    element->method_event (element, interface, event, content_region);
}
