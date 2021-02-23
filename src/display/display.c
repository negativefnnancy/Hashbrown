#include <stdlib.h>

#include <hashbrown/display/display.h>

void display_init (display_t *display, unsigned int width, unsigned int height) {

    buffer_init (&display->output, width, height);
}

void display_deinit (display_t *display) {

    buffer_deinit (&display->output);
}
