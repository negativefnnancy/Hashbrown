#ifndef UTIL_H
#define UTIL_H

#include <SDL2/SDL.h>

#include "region.h"
#include "color.h"

double clamp (double value, double min, double max);
/* TODO more consistence naming */
void draw_box (SDL_Renderer *renderer, region_t clipping_region, region_t outer, region_t inner, color_t color);

#endif /* UTIL_H */
