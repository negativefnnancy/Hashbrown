#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

typedef struct display_t {

    /* size of the screen in pixels */
    unsigned int width, height;

    /* the buffer of phosphor */
    double *buffer_phosphor;

    /* the output image in RGBA8888 format */
    uint32_t *buffer_output;

} display_t;

void display_init    (display_t *display,
                      unsigned int width,
                      unsigned int height);
void display_deinit  (display_t *display);
void display_process (display_t *display, double time);

#endif /* DISPLAY_H */
