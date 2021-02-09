#include <math.h>

#include "vector.h"

vec2_t vec2_make (double x, double y) {

    vec2_t vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

vec2_t vec2_add (vec2_t a, vec2_t b) {

    return vec2_make (a.x + b.x, a.y + b.y);
}

vec2_t vec2_subtract (vec2_t a, vec2_t b) {

    return vec2_make (a.x - b.x, a.y - b.y);
}

vec2_t vec2_multiply (vec2_t a, vec2_t b) {

    return vec2_make (a.x * b.x, a.y * b.y);
}

vec2_t vec2_divide (vec2_t a, vec2_t b) {

    return vec2_make (a.x / b.x, a.y / b.y);
}

vec2_t vec2_add_scalar (vec2_t vector, double scalar) {

    return vec2_make (vector.x + scalar, vector.y + scalar);
}

vec2_t vec2_subtract_scalar (vec2_t vector, double scalar) {

    return vec2_make (vector.x - scalar, vector.y - scalar);
}

vec2_t vec2_multiply_scalar (vec2_t vector, double scalar) {

    return vec2_make (vector.x * scalar, vector.y * scalar);
}

vec2_t vec2_divide_scalar (vec2_t vector, double scalar) {

    return vec2_make (vector.x / scalar, vector.y / scalar);
}

double vec2_length (vec2_t vector) {

    return sqrt (vector.x * vector.x + vector.y * vector.y);
}

vec2_t vec2_unit (vec2_t vector) {

    return vec2_divide_scalar (vector, vec2_length (vector));
}

double vec2_dot_product (vec2_t a, vec2_t b) {

    return a.x * b.x + a.y * b.y;
}

vec2_t vec2_min (vec2_t a, vec2_t b) {

    return vec2_make (fmin (a.x, b.x), fmin (a.y, b.y));
}

vec2_t vec2_max (vec2_t a, vec2_t b) {

    return vec2_make (fmax (a.x, b.x), fmax (a.y, b.y));
}
