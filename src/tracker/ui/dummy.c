#include <stdlib.h>
#include <stdio.h>

#include "dummy.h"
#include "../interface.h"

void ui_box_draw_method_dummy (ui_box_t *element,
                               interface_t *interface,
                               region_t clipping_region,
                               region_t element_region) {

    ui_dummy_draw ((ui_dummy_t *) element,
                   interface,
                   clipping_region,
                   element_region);
}

void ui_box_event_method_dummy (ui_box_t *element,
                                interface_t *interface,
                                ui_event_t event,
                                region_t element_region) {

    ui_dummy_event ((ui_dummy_t *) element,
                    interface,
                    event,
                    element_region);
}

void ui_dummy_init (ui_dummy_t *element, color_t color, ui_box_style_t style) {

    ui_box_init ((ui_box_t *) element,
                 ui_box_draw_method_dummy,
                 ui_box_event_method_dummy,
                 style);
    element->color = color;
}

void ui_dummy_draw (ui_dummy_t *element,
                    interface_t *interface,
                    region_t clipping_region,
                    region_t element_region) {

    SDL_Rect rect;
    region_t intersection;
    
    intersection = region_intersection (element_region, clipping_region);
    rect = sdl_rect_from_region (intersection);

    sdl_renderer_set_color (interface->renderer, element->color);
    SDL_RenderFillRect (interface->renderer, &rect);
}

void ui_dummy_event (ui_dummy_t *element,
                     interface_t *interface,
                     ui_event_t event,
                     region_t element_region) {

    switch (event.type) {

        case EVENT_MOUSE:

            switch (event.event.mouse.type) {

                case EVENT_MOUSE_CLICK:
                    puts ("mouse click event");
                    break;

                case EVENT_MOUSE_BUTTON:
                    puts ("mouse button event");
                    break;

                case EVENT_MOUSE_MOVE:
                    puts ("mouse move event");
                    break;

                default:
                    break;
            }

            break;

        case EVENT_KEYBOARD:
            puts ("keyboard event");
            break;

        default:
            break;
    }
}

ui_dummy_t *ui_dummy_create (color_t color, ui_box_style_t style) {

    ui_dummy_t *element = malloc (sizeof (ui_dummy_t));
    ui_dummy_init (element, color, style);
    return element;
}

void ui_dummy_destroy (ui_dummy_t *element) {

    free (element);
}
