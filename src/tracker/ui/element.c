#include "element.h"
#include "../interface.h"

void ui_element_draw (ui_element_t *element,
                      interface_t *interface,
                      region_t clipping_region,
                      region_t element_region) {

    element->method_draw (element, interface, clipping_region, element_region);
}
