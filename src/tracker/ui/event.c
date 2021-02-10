#include "event.h"
#include "element.h"
#include "../interface.h"

ui_event_t ui_event_make_mouse (ui_mouse_event_type_t type,
                                vec2_t position,
                                int button,
                                bool pressed) {

    ui_event_t event;
    event.type = EVENT_MOUSE;
    event.event.mouse.type     = type;
    event.event.mouse.position = position;
    event.event.mouse.button   = button;
    event.event.mouse.pressed  = pressed;
    return event;
}

ui_event_t ui_event_make_keyboard (int key, bool pressed) {

    ui_event_t event;
    event.type = EVENT_KEYBOARD;
    event.event.keyboard.key      = key;
    event.event.keyboard.pressed  = pressed;
    return event;
}

void ui_event_propagate (ui_event_t event,
                         interface_t *interface,
                         ui_element_t *element,
                         region_t parent_region,
                         region_t child_region) {

    /* offset the event position */
    if (event.type == EVENT_MOUSE) {

        double offset = vec2_distance (child_region.position, parent_region.position);
        event.event.mouse.position 
            = vec2_subtract_scalar (event.event.mouse.position, offset);
    }

    /* handle the event the content */
    element->method_event (element, interface, event, child_region);
}
