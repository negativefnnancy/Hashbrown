#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include <stdlib.h>

#include "box.h"
#include "layout.h"

typedef struct ui_container_t {

    ui_box_t base;
    ui_layout_t *layout;
    size_t n_children;
    ui_element_t **children;
    ui_element_t *hovered;
    ui_element_t *selected;

} ui_container_t;

void ui_container_init (ui_container_t *element,
                        ui_box_style_t style,
                        ui_layout_t *layout,
                        size_t n_children,
                        ui_element_t **children);

void ui_container_draw (ui_container_t *element,
                        struct interface_t *interface,
                        region_t clipping_region,
                        region_t element_region);

void ui_container_event (ui_container_t *element,
                         struct interface_t *interface,
                         ui_event_t event,
                         region_t element_region);

ui_container_t *ui_container_create  (ui_box_style_t style,
                                      ui_layout_t *layout,
                                      size_t n_children,
                                      ui_element_t **children);
void            ui_container_destroy (ui_container_t *element);

#endif /* UI_CONTAINER_H */
