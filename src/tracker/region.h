#ifndef REGION_H
#define REGION_H

#include <SDL2/SDL.h>

#include "vector.h"

typedef struct region_t {

    vec2_t position;
    vec2_t dimensions;

} region_t;

region_t region_make         (double x, double y, double width, double height);
region_t region_make_corners (double x1, double y1, double x2, double y2);
region_t region_from_corners (vec2_t top_left, vec2_t bottom_right);

void region_get_corners (region_t region, vec2_t *top_left, vec2_t *bottom_right);

region_t region_bounds       (region_t a, region_t b);
region_t region_intersection (region_t a, region_t b);

SDL_Rect sdl_rect_from_region (region_t region);

#endif /* REGION_H */
