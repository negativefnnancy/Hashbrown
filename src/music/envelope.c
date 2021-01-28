#include <stdlib.h>
#include <string.h>

#include <hashbrown/music/envelope.h>

void envelope_init (envelope_t *envelope,
                    size_t n_samples,
                    envelope_loop_mode_t loop_mode) {

    memset (envelope, 0, sizeof (envelope_t));
    envelope->samples = calloc (n_samples, sizeof (envelope_sample_t));
    envelope->n_samples = n_samples;
    envelope->loop_mode = loop_mode;
}

void envelope_deinit (envelope_t *envelope) {

    free (envelope->samples);
}
