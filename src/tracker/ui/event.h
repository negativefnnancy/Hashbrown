#ifndef UI_EVENT_H
#define UI_EVENT_H

#include <stdbool.h>

#include "../vector.h"
#include "../region.h"

struct interface_t;
struct ui_element_t;

typedef enum ui_event_type_t {

    EVENT_MOUSE,
    EVENT_KEYBOARD,

    N_EVENT_TYPE

} ui_event_type_t;

typedef enum ui_mouse_event_type_t {

    EVENT_MOUSE_BUTTON,
    EVENT_MOUSE_MOVE,

    N_MOUSE_EVENT_TYPE

} ui_mouse_event_type_t;

typedef struct ui_event_t {

    ui_event_type_t type;

    union {

        struct {

            ui_mouse_event_type_t type;
            vec2_t position;
            int button;
            bool pressed;

        } mouse;

        struct {

            int key;
            bool pressed;

        } keyboard;

    } event;

} ui_event_t;

ui_event_t ui_event_make_mouse (ui_mouse_event_type_t type,
                                vec2_t position,
                                int button,
                                bool pressed);
ui_event_t ui_event_make_keyboard (int key, bool pressed);

void ui_event_propagate (ui_event_t event,
                         struct interface_t *interface,
                         struct ui_element_t *element,
                         region_t parent_region,
                         region_t child_region);

#endif /* UI_EVENT_H */
