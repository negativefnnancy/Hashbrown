#ifndef UI_LAYOUT_SPLIT_H
#define UI_LAYOUT_SPLIT_H

#include "layout.h"

typedef enum split_orientation_t {

    SPLIT_ORIENTATION_HORIZONTAL,
    SPLIT_ORIENTATION_VERTICAL,

    N_SPLIT_ORIENTATION

} split_orientation_t;

typedef struct ui_layout_split_t {

    ui_layout_t base;
    split_orientation_t orientation;
    double spacing;
    double *lengths;

} ui_layout_split_t;

void ui_layout_split_init (ui_layout_split_t *layout,
                           split_orientation_t orientation,
                           double spacing,
                           double *lengths);

void ui_layout_split_iterator_init (ui_layout_iterator_t *iterator);

int ui_layout_split_iterator_iterate (ui_layout_iterator_t *iterator,
                                      ui_element_t **element,
                                      region_t *element_region);

ui_layout_split_t *ui_layout_split_create  (split_orientation_t orientation,
                                            double spacing,
                                            double *lengths);
void               ui_layout_split_destroy (ui_layout_split_t *layout);

#endif /* UI_LAYOUT_SPLIT_H */
