#include <stdlib.h>

#include <hashbrown/display/buffer.h>

void buffer_init (buffer_t *buffer, unsigned int width, unsigned int height) {

    buffer->width  = width;
    buffer->height = height;
    buffer->pixels = calloc (width * height, sizeof (double));
}

void buffer_deinit (buffer_t *buffer) {

    free (buffer->pixels);
}
