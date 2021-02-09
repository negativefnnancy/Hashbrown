#include "region.h"

region_t region_make (double x, double y, double width, double height) {

    region_t region;
    region.position   = vec2_make (x, y);
    region.dimensions = vec2_make (width, height);
    return region;
}

region_t region_make_corners (double x1, double y1, double x2, double y2) {

    region_t region;
    region.position   = vec2_make (x1,      y1);
    region.dimensions = vec2_make (x2 - x1, y2 - x1);
    return region;
}

region_t region_from_corners (vec2_t top_left, vec2_t bottom_right) {

    region_t region;
    region.position   = top_left;
    region.dimensions = vec2_subtract (bottom_right, top_left);
    return region;
}

void region_get_corners (region_t region, vec2_t *top_left, vec2_t *bottom_right) {

    *top_left     = region.position;
    *bottom_right = vec2_add (region.position, region.dimensions);
}

region_t region_bounds (region_t a, region_t b) {

    vec2_t top_left_a,      bottom_right_a;
    vec2_t top_left_b,      bottom_right_b;
    vec2_t top_left_bounds, bottom_right_bounds;
    
    region_get_corners (a, &top_left_a, &bottom_right_a);
    region_get_corners (b, &top_left_b, &bottom_right_b);

    top_left_bounds     = vec2_min (top_left_a,     top_left_b);
    bottom_right_bounds = vec2_max (bottom_right_a, bottom_right_b);

    return region_from_corners (top_left_bounds, bottom_right_bounds);
}

region_t region_intersection (region_t a, region_t b) {

    vec2_t top_left_a,      bottom_right_a;
    vec2_t top_left_b,      bottom_right_b;
    vec2_t top_left_bounds, bottom_right_bounds;
    
    region_get_corners (a, &top_left_a, &bottom_right_a);
    region_get_corners (b, &top_left_b, &bottom_right_b);

    top_left_bounds     = vec2_max (top_left_a,     top_left_b);
    bottom_right_bounds = vec2_min (bottom_right_a, bottom_right_b);

    return region_from_corners (top_left_bounds, bottom_right_bounds);
}

SDL_Rect sdl_rect_from_region (region_t region) {

    SDL_Rect rect;
    rect.x = region.position.x;
    rect.y = region.position.y;
    rect.w = region.dimensions.x;
    rect.h = region.dimensions.y;
    return rect;
}
