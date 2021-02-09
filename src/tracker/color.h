#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>

typedef struct color_t {

    double red;
    double green;
    double blue;

} color_t;

color_t color_make (double red, double green, double blue);

color_t color_clip (color_t color);

color_t color_add      (color_t a, color_t b);
color_t color_multiply (color_t a, color_t b);

void sdl_renderer_set_color (SDL_Renderer *renderer, color_t color);

#endif /* COLOR_H */
