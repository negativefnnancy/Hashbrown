#include "layout.h"

void ui_layout_init (ui_layout_t *layout,
                     ui_layout_iterator_init_method_t    *iterator_method_init,
                     ui_layout_iterator_iterate_method_t *iterator_method_iterate) {

    layout->iterator_method_init    = iterator_method_init;
    layout->iterator_method_iterate = iterator_method_iterate;
}

void ui_layout_iterator_init (ui_layout_iterator_t *iterator,
                              ui_layout_t *layout,
                              size_t n_elements,
                              ui_element_t **elements,
                              region_t region) {

    iterator->layout     = layout;
    iterator->i_element = 0;
    iterator->n_elements = n_elements;
    iterator->elements   = elements;
    iterator->region     = region;
    layout->iterator_method_init (iterator);
}

int ui_layout_iterator_iterate (ui_layout_iterator_t *iterator,
                                ui_element_t **element,
                                region_t *element_region) {

    return iterator->layout->iterator_method_iterate (iterator, element, element_region);
}
