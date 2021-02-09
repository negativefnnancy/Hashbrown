#ifndef UI_DUMMY_H
#define UI_DUMMY_H

#include "box.h"
#include "../color.h"

typedef struct ui_dummy_t {

    ui_box base;
    color_t color;

} ui_dummy_t;

void ui_dummy_draw (ui_dummy_t *element,
                    struct interface_t *interface,
                    region_t clipping_region,
                    region_t element_region);

#endif /* UI_DUMMY_H */
