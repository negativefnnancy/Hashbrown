#include <stdlib.h>

#include <hashbrown/display/display.h>

void display_init (display_t *display, unsigned int width, unsigned int height) {

    display->width  = width;
    display->height = height;
    display->buffer_phosphor = calloc (width * height, sizeof (double));
    display->buffer_output   = calloc (width * height, sizeof (uint32_t));
}

void display_deinit (display_t *display) {

    free (display->buffer_phosphor);
    free (display->buffer_output);
}

void display_process (display_t *display, double time) {

    printf ("%lf\n", time);
}
