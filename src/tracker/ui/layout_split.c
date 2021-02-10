#include <stdlib.h>

#include "layout_split.h"

void ui_layout_split_init (ui_layout_split_t *layout,
                           split_orientation_t orientation,
                           double spacing,
                           double *lengths) {

    ui_layout_init ((ui_layout_t *) layout,
                    ui_layout_split_iterator_init,
                    ui_layout_split_iterator_iterate);
    layout->orientation = orientation;
    layout->spacing     = spacing;
    layout->lengths     = lengths;
}

void ui_layout_split_iterator_init (ui_layout_iterator_t *iterator) {

    ui_layout_split_t *layout = (ui_layout_split_t *) iterator->layout;
}

double get_length (double length, double remaining_length) {

    return length != 0 ? length : remaining_length;
}

int ui_layout_split_iterator_iterate (ui_layout_iterator_t *iterator,
                                      ui_element_t **element,
                                      region_t *element_region) {

    /* TODO implement an iterator context so this doesnt have to be recalculated every time..... */

    size_t i;
    double length;
    double x = 0;
    double sum = 0;
    double remaining = 0;
    vec2_t position = iterator->region.position;
    vec2_t dimensions = iterator->region.dimensions;
    ui_layout_split_t *layout = (ui_layout_split_t *) iterator->layout;

    if (iterator->i_element >= iterator->n_elements)
        return 0;

    /* calculate total explicit space */
    for (i = 0; i < iterator->n_elements; i++)
        sum += layout->lengths[i] + layout->spacing;
    sum -= layout->spacing;
    
    /* calculate remaining space */
    switch (layout->orientation) {

        case SPLIT_ORIENTATION_HORIZONTAL:
            remaining = dimensions.x - sum;
            break;

        case SPLIT_ORIENTATION_VERTICAL:
            remaining = dimensions.y - sum;
            break;

        default:
            break;
    }

    /* find this position */
    for (i = 0; i < iterator->i_element; i++)
        x += get_length (layout->lengths[i], remaining) + layout->spacing;

    /* get the length */
    length = get_length (layout->lengths[iterator->i_element], remaining);

    /* get the element */
    *element = iterator->elements[iterator->i_element++];

    /* set the region */
    switch (layout->orientation) {

        case SPLIT_ORIENTATION_HORIZONTAL:
            *element_region = region_make (position.x + x,
                                           position.y,
                                           length,
                                           dimensions.y);
            break;

        case SPLIT_ORIENTATION_VERTICAL:
            *element_region = region_make (position.x,
                                           position.y + x,
                                           dimensions.x,
                                           length);
            break;

        default:
            break;
    }

    /* report how many elements remain to be iterated */
    return iterator->n_elements - iterator->i_element + 1;
}

ui_layout_split_t *ui_layout_split_create (split_orientation_t orientation,
                                           double spacing,
                                           double *lengths) {

    ui_layout_split_t *layout = malloc (sizeof (ui_layout_split_t));
    ui_layout_split_init (layout, orientation, spacing, lengths);
    return layout;
}

void ui_layout_split_destroy (ui_layout_split_t *layout) {

    free (layout);
}
