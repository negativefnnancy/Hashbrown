#include "util.h"

double clamp (double value, double min, double max) {

    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

void draw_box (SDL_Renderer *renderer, region_t clipping_region, region_t outer, region_t inner, color_t color) {

    /* TODO do this more generally and betterly and more efficientlyly later */

    region_t intersection = region_intersection (outer, clipping_region);
    SDL_Rect rect = sdl_rect_from_region (intersection);
    sdl_renderer_set_color (renderer, color);
    SDL_RenderFillRect (renderer, &rect);
}
