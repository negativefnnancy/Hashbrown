#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "../region.h"

struct interface_t;

typedef struct ui_element_t {

    int tmp;

} ui_element_t;

void ui_element_draw (ui_element_t *element,
                      struct interface_t *interface,
                      region_t clipping_region,
                      region_t element_region);

#endif /* UI_ELEMENT_H */
