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
