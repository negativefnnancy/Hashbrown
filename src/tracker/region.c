#include "region.h"

region_t region_make (double x, double y, double width, double height) {

    region_t region;
    region.position   = vec2_make (x, y);
    region.dimensions = vec2_make (width, height);
    return region;
}

region_t region_make_from_vectors (vec2_t position, vec2_t dimensions) {

    region_t region;
    region.position   = position;
    region.dimensions = dimensions;
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

bool region_contains_point (region_t region, vec2_t point) {

    vec2_t top_left, bottom_right;

    region_get_corners (region, &top_left, &bottom_right);

    return point.x >= top_left.x && point.x < bottom_right.x &&
           point.y >= top_left.y && point.y < bottom_right.y;
}

bool region_contains_mouse (region_t region) {

    int x, y;
    SDL_GetMouseState (&x, &y);
    return region_contains_point (region, vec2_make (x, y));
}

region_t region_inset (region_t region, double amount) {

    vec2_t top_left_outer, bottom_right_outer;
    vec2_t top_left_inner, bottom_right_inner;

    region_get_corners (region, &top_left_outer, &bottom_right_outer);
    top_left_inner     = vec2_add_scalar      (top_left_outer, amount);
    bottom_right_inner = vec2_subtract_scalar (bottom_right_outer, amount);

    return region_from_corners (top_left_inner, bottom_right_inner);
}

SDL_Rect sdl_rect_from_region (region_t region) {

    SDL_Rect rect;
    rect.x = region.position.x;
    rect.y = region.position.y;
    rect.w = region.dimensions.x;
    rect.h = region.dimensions.y;
    /* TODO temp? */
    if (rect.w < 0)
        rect.w = 0;
    if (rect.h < 0)
        rect.h = 0;
    return rect;
}
