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
                   char *text,
                   text_alignment_t alignment) {

    ui_box_init ((ui_box_t *) element,
                 ui_box_draw_method_text,
                 ui_box_event_method_text,
                 style);
    element->color_foreground = color_foreground;
    element->color_background = color_background;
    element->text             = text;
    element->alignment        = alignment;
}

void ui_text_draw (ui_text_t *element,
                   interface_t *interface,
                   region_t clipping_region,
                   region_t element_region) {

    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
    region_t intersection;
    
    intersection = region_intersection (element_region, clipping_region);
    rect = sdl_rect_from_region (intersection);

    surface = TTF_RenderText_Shaded (interface->font,
                                     element->text,
                                     sdl_color_from_color (element->color_foreground),
                                     sdl_color_from_color (element->color_background));

    texture = SDL_CreateTextureFromSurface (interface->renderer, surface);

    SDL_RenderCopy (interface->renderer, texture, NULL, &rect);

    SDL_FreeSurface (surface);
    SDL_DestroyTexture (texture);
}

void ui_text_event (ui_text_t *element,
                    interface_t *interface,
                    ui_event_t event,
                    region_t element_region) {

    /* TODO */
}

ui_text_t *ui_text_create (ui_box_style_t style,
                           color_t color_foreground,
                           color_t color_background,
                           char *text,
                           text_alignment_t alignment) {

    ui_text_t *element = malloc (sizeof (ui_text_t));
    ui_text_init (element,
                  style,
                  color_foreground,
                  color_background,
                  text,
                  alignment);
    return element;
}

void ui_text_destroy (ui_text_t *element) {

    free (element);
}
