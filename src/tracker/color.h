#ifndef COLOR_H
#define COLOR_H

typedef struct color_t {

    double red;
    double green;
    double blue;

} color_t;

color_t color_make (double red, double green, double blue);

color_t color_clip (color_t color);

color_t color_add      (color_t a, color_t b);
color_t color_multiply (color_t a, color_t b);

#endif /* COLOR_H */
