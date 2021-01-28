#include <stdlib.h>

#include <hashbrown/music/channel.h>

void channel_init (channel_t *channel, size_t n_patterns) {

    channel->i_patterns = calloc (n_patterns, sizeof (size_t));
    channel->n_patterns = n_patterns;
}

void channel_deinit (channel_t *channel) {

    free (channel->i_patterns);
}
