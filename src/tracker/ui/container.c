#include <stdlib.h>

#include "container.h"
#include "../interface.h"

void ui_box_draw_method_container (ui_box_t *element,
                                   interface_t *interface,
                                   region_t clipping_region,
                                   region_t element_region) {

    ui_container_draw ((ui_container_t *) element,
                       interface,
                       clipping_region,
                       element_region);
}

void ui_box_event_method_container (ui_box_t *element,
                                    interface_t *interface,
                                    ui_event_t event,
                                    region_t element_region) {

    ui_container_event ((ui_container_t *) element,
                        interface,
                        event,
                        element_region);
}

void ui_container_init (ui_container_t *element,
                        ui_box_style_t style,
                        ui_layout_t *layout,
                        size_t n_children,
                        ui_element_t **children) {

    ui_box_init ((ui_box_t *) element,
                 ui_box_draw_method_container,
                 ui_box_event_method_container,
                 style);
    element->layout     = layout;
    element->n_children = n_children;
    element->children   = children;
    element->hovered    = NULL;
    element->selected   = NULL;
}

void ui_container_draw (ui_container_t *element,
                        interface_t *interface,
                        region_t clipping_region,
                        region_t element_region) {

    ui_element_t *child;
    region_t child_region;
    ui_layout_iterator_t iterator;

    ui_layout_iterator_init (&iterator,
                             element->layout,
                             element->n_children,
                             element->children,
                             element_region);

    while (ui_layout_iterator_iterate (&iterator, &child, &child_region))
        ui_element_draw (child, interface, clipping_region, child_region);
}

ui_element_t *ui_container_child_at_point (ui_container_t *element,
                                           region_t element_region,
                                           vec2_t point) {

    ui_element_t *child;
    region_t child_region;
    ui_layout_iterator_t iterator;

    ui_layout_iterator_init (&iterator,
                             element->layout,
                             element->n_children,
                             element->children,
                             element_region);

    while (ui_layout_iterator_iterate (&iterator, &child, &child_region))
        if (region_contains_point (child_region, point))
            return child;

    return NULL;
}

region_t ui_container_child_region (ui_container_t *element,
                                    region_t element_region,
                                    ui_element_t *target) {

    ui_element_t *child;
    region_t child_region;
    ui_layout_iterator_t iterator;

    ui_layout_iterator_init (&iterator,
                             element->layout,
                             element->n_children,
                             element->children,
                             element_region);

    while (ui_layout_iterator_iterate (&iterator, &child, &child_region))
        if (child == target)
            return child_region;

    /* TODO a better error not found */
    return element_region;
}

void ui_container_event (ui_container_t *element,
                         interface_t *interface,
                         ui_event_t event,
                         region_t element_region) {

    switch (event.type) {

        case EVENT_MOUSE:

            switch (event.event.mouse.type) {

                /* on mouse move, find the hovered child */
                case EVENT_MOUSE_MOVE:

                    element->hovered
                        = ui_container_child_at_point (element,
                                                       element_region,
                                                       event.event.mouse.position);
                    break;

                /* on mouse down, set selected to hovered child */
                case EVENT_MOUSE_BUTTON:

                    if (event.event.mouse.pressed)
                        element->selected = element->hovered;
                    break;

                default:
                    break;
            }

            /* send mouse events to the hovered child */
            if (element->hovered)
                ui_event_propagate (event,
                                    interface,
                                    element->hovered,
                                    element_region,
                                    ui_container_child_region (element,
                                                               element_region,
                                                               element->hovered));
            break;

        case EVENT_KEYBOARD:

            /* send keyboard events to the selected child */
            if (element->selected)
                ui_event_propagate (event,
                                    interface,
                                    element->selected,
                                    element_region,
                                    ui_container_child_region (element,
                                                               element_region,
                                                               element->selected));
            break;

        default:
            break;
    }
}

ui_container_t *ui_container_create (ui_box_style_t style,
                                     ui_layout_t *layout,
                                     size_t n_children,
                                     ui_element_t **children) {

    ui_container_t *element = malloc (sizeof (ui_container_t));
    ui_container_init (element, style, layout, n_children, children);
    return element;
}

void ui_container_destroy (ui_container_t *element) {

    free (element);
}
