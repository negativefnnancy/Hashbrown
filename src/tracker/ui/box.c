#include "box.h"
#include "../interface.h"

void ui_element_draw_method_box (ui_element_t *element,
                                 interface_t *interface,
                                 region_t clipping_region,
                                 region_t element_region) {

    ui_box_draw ((ui_box_t *) element,
                 interface,
                 clipping_region,
                 element_region);
}

void ui_box_draw (ui_box_t *element,
                  interface_t *interface,
                  region_t clipping_region,
                  region_t element_region) {

    /* TODO draw the border */

    /* jump to the subclass draw method */
    element->method_draw (element, interface, clipping_region, element_region);
}
