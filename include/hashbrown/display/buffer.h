#ifndef BUFFER_H
#define BUFFER_H

typedef struct buffer_t {

    unsigned int width, height;
    double *pixels;

} buffer_t;

void buffer_init   (buffer_t *buffer,
                    unsigned int width,
                    unsigned int height);
void buffer_deinit (buffer_t *buffer);

#endif /* BUFFER_H */
