#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdlib.h>

typedef struct channel_t {

    size_t n_patterns;
    size_t *i_patterns;

} channel_t;

void channel_init   (channel_t *channel, size_t n_patterns);
void channel_deinit (channel_t *channel);

#endif /* CHANNEL_H */
