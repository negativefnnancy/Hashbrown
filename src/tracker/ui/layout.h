#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

#include <stdlib.h>

#include "element.h"
#include "../region.h"

struct ui_layout_t;
struct ui_layout_iterator_t;

typedef void ui_layout_iterator_init_method_t (struct ui_layout_iterator_t *iterator);

typedef int ui_layout_iterator_iterate_method_t (struct ui_layout_iterator_t *iterator,
                                                 ui_element_t **element,
                                                 region_t *element_region);

typedef struct ui_layout_t {

    ui_layout_iterator_init_method_t    *iterator_method_init;
    ui_layout_iterator_iterate_method_t *iterator_method_iterate;

} ui_layout_t;

void ui_layout_init (ui_layout_t *layout,
                     ui_layout_iterator_init_method_t    *iterator_method_init,
                     ui_layout_iterator_iterate_method_t *iterator_method_iterate);

typedef struct ui_layout_iterator_t {

    ui_layout_t *layout;
    size_t i_element;
    size_t n_elements;
    ui_element_t **elements;
    region_t region;

} ui_layout_iterator_t;

void ui_layout_iterator_init (ui_layout_iterator_t *iterator,
                              ui_layout_t *layout,
                              size_t n_elements,
                              ui_element_t **elements,
                              region_t region);

ui_layout_iterator_iterate_method_t ui_layout_iterator_iterate;

#endif /* UI_LAYOUT_H */
