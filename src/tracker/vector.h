#ifndef VECTOR_H
#define VECTOR_H

typedef struct vec2_t {

    double x;
    double y;

} vec2_t;

vec2_t vec2_make (double x, double y);

vec2_t vec2_add      (vec2_t a, vec2_t b);
vec2_t vec2_subtract (vec2_t a, vec2_t b);
vec2_t vec2_multiply (vec2_t a, vec2_t b);
vec2_t vec2_divide   (vec2_t a, vec2_t b);

vec2_t vec2_add_scalar      (vec2_t vector, double scalar);
vec2_t vec2_subtract_scalar (vec2_t vector, double scalar);
vec2_t vec2_multiply_scalar (vec2_t vector, double scalar);
vec2_t vec2_divide_scalar   (vec2_t vector, double scalar);

double vec2_length   (vec2_t vector);
double vec2_distance (vec2_t a, vec2_t b);
vec2_t vec2_unit     (vec2_t vector);

double vec2_dot_product (vec2_t a, vec2_t b);

vec2_t vec2_min (vec2_t a, vec2_t b);
vec2_t vec2_max (vec2_t a, vec2_t b);

#endif /* VECTOR_H */
