#include "color.h"
#include "util.h"

color_t color_make (double red, double green, double blue) {

    color_t color;
    color.red   = red;
    color.green = green;
    color.blue  = blue;
    return color;
}

color_t color_clip (color_t color) {

    color.red   = clamp (color.red,   0, 1);
    color.green = clamp (color.green, 0, 1);
    color.blue  = clamp (color.blue,  0, 1);
    return color;
}

color_t color_add (color_t a, color_t b) {

    double red   = a.red   + b.red;
    double green = a.green + b.green;
    double blue  = a.blue  + b.blue;
    return color_make (red, green, blue);
}

color_t color_multiply (color_t a, color_t b) {

    double red   = a.red   * b.red;
    double green = a.green * b.green;
    double blue  = a.blue  * b.blue;
    return color_make (red, green, blue);
}
