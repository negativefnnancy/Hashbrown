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

void ui_container_init (ui_container_t *element,
                        ui_box_style_t style,
                        ui_layout_t *layout,
                        size_t n_children,
                        ui_element_t **children) {

    ui_box_init ((ui_box_t *) element, ui_box_draw_method_container, style);
    element->layout     = layout;
    element->n_children = n_children;
    element->children   = children;
}

void ui_container_draw (ui_container_t *element,
                        struct interface_t *interface,
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
