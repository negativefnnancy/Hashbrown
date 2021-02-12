#include <stdlib.h>

#include "text.h"
#include "../interface.h"

void ui_box_draw_method_text (ui_box_t *element,
                              interface_t *interface,
                              region_t clipping_region,
                              region_t element_region) {

    ui_text_draw ((ui_text_t *) element,
                  interface,
                  clipping_region,
                  element_region);
}

void ui_box_event_method_text (ui_box_t *element,
                               interface_t *interface,
                               ui_event_t event,
                               region_t element_region) {

    ui_text_event ((ui_text_t *) element,
                   interface,
                   event,
                   element_region);
}

void ui_text_init (ui_text_t *element,
                   ui_box_style_t style,
                   color_t color_foreground,
                   color_t color_background,
                   color_t color_hovered,
                   color_t color_pressed,
                   char *text,
                   text_alignment_t alignment) {

    ui_box_init ((ui_box_t *) element,
                 ui_box_draw_method_text,
                 ui_box_event_method_text,
                 style);
    element->color_foreground = color_foreground;
    element->color_background = color_background;
    element->color_hovered    = color_hovered;
    element->color_pressed    = color_pressed;
    element->text             = text;
    element->alignment        = alignment;
}

region_t align_regions (region_t region_frame,
                        region_t region_target,
                        text_alignment_t alignment) {

    vec2_t position_frame, position_target;
    double x_frame, y_frame, x_target, y_target;

    /* align vertically centered */
    y_frame  = region_frame.position.y  + region_frame.dimensions.y  / 2;
    y_target = region_target.position.y + region_target.dimensions.y / 2;

    /* align horizontally */
    switch (alignment) {

        case TEXT_ALIGNMENT_LEFT:
            x_frame  = region_frame.position.x;
            x_target = region_target.position.x;
            break;

        case TEXT_ALIGNMENT_RIGHT:
            x_frame  = region_frame.position.x  + region_frame.dimensions.x;
            x_target = region_target.position.x + region_target.dimensions.x;
            break;

        case TEXT_ALIGNMENT_CENTER:
            x_frame  = region_frame.position.x  + region_frame.dimensions.x  / 2;
            x_target = region_target.position.x + region_target.dimensions.x / 2;
            break;

        default:
            /* TODO handle this better????? */
            break;
    }

    position_frame  = vec2_make (x_frame,  y_frame);
    position_target = vec2_make (x_target, y_target);

    /* translate the target region so the alignment positions match */
    region_target.position
        = vec2_add (region_target.position,
                    vec2_subtract (position_frame, position_target));
    return region_target;
}

/* TODO really change these namings for the regions in all the other places */
void ui_text_draw (ui_text_t *element,
                   interface_t *interface,
                   region_t region_clipping,
                   region_t region_element) {

    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect_source,
             rect_destination,
             rect_destination_background;
    region_t region_surface,
             region_text,
             region_source,
             region_destination,
             region_destination_background;
    color_t color;

    if (region_contains_mouse (region_element))
        color = SDL_GetMouseState (NULL, NULL) & SDL_BUTTON (SDL_BUTTON_LEFT)
              ? element->color_pressed
              : element->color_hovered;
    
    else
        color = element->color_background;
    
    surface = TTF_RenderText_Shaded (interface->font,
                                     element->text,
                                     sdl_color_from_color (element->color_foreground),
                                     sdl_color_from_color (color));

    region_surface = region_make (0, 0, surface->w, surface->h);
    region_text = align_regions (region_element, region_surface, element->alignment);
    region_destination = region_intersection (region_text,        region_element);
    region_destination = region_intersection (region_destination, region_clipping);
    region_source 
        = region_make_from_vectors (vec2_subtract (region_destination.position,
                                                   region_text.position),
                                    region_destination.dimensions);
    region_destination_background = region_intersection (region_element, region_clipping);  

    rect_source                 = sdl_rect_from_region (region_source);
    rect_destination            = sdl_rect_from_region (region_destination);
    rect_destination_background = sdl_rect_from_region (region_destination_background);

    sdl_renderer_set_color (interface->renderer, color);
    SDL_RenderFillRect (interface->renderer, &rect_destination_background);

    texture = SDL_CreateTextureFromSurface (interface->renderer, surface);
    SDL_RenderCopy (interface->renderer, texture, &rect_source, &rect_destination);

    SDL_FreeSurface (surface);
    SDL_DestroyTexture (texture);
}

void ui_text_event (ui_text_t *element,
                    interface_t *interface,
                    ui_event_t event,
                    region_t element_region) {

    /* TODO implement button and text editing */

    switch (event.type) {

        case EVENT_MOUSE:

            switch (event.event.mouse.type) {

                case EVENT_MOUSE_CLICK:
                    puts ("TXT mouse click event");
                    break;

                case EVENT_MOUSE_BUTTON:
                case EVENT_MOUSE_ENTER:
                case EVENT_MOUSE_EXIT:
                    interface_repaint (interface, element_region);
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

ui_text_t *ui_text_create (ui_box_style_t style,
                           color_t color_foreground,
                           color_t color_background,
                           color_t color_hovered,
                           color_t color_pressed,
                           char *text,
                           text_alignment_t alignment) {

    ui_text_t *element = malloc (sizeof (ui_text_t));
    ui_text_init (element,
                  style,
                  color_foreground,
                  color_background,
                  color_hovered,
                  color_pressed,
                  text,
                  alignment);
    return element;
}

void ui_text_destroy (ui_text_t *element) {

    free (element);
}
