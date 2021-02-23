#ifndef DISPLAY_H
#define DISPLAY_H

#include "buffer.h"

typedef struct display_t {

    buffer_t output;

} display_t;

void display_init    (display_t *display,
                      unsigned int width,
                      unsigned int height);
void display_deinit  (display_t *display);
void display_process (display_t *display);

#endif /* DISPLAY_H */
