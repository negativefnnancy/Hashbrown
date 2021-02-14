#include <stdlib.h>
#include <string.h>

#include <hashbrown/music/module.h>

void module_init (module_t *module,
                  char *title,
                  char *composer,
                  char *description,
                  size_t n_channels,
                  size_t n_patterns,
                  size_t n_envelopes) {

    module->title       = malloc (MAX_TITLE_LENGTH);
    module->composer    = malloc (MAX_COMPOSER_LENGTH);
    module->description = malloc (MAX_DESCRIPTION_LENGTH);
    strncpy (module->title,       title,       MAX_TITLE_LENGTH);
    strncpy (module->composer,    composer,    MAX_COMPOSER_LENGTH);
    strncpy (module->description, description, MAX_DESCRIPTION_LENGTH);
    module->channels  = calloc (n_channels,  sizeof (channel_t));
    module->patterns  = calloc (n_patterns,  sizeof (pattern_t));
    module->envelopes = calloc (n_envelopes, sizeof (envelope_t));
    module->n_channels  = n_channels;
    module->n_patterns  = n_patterns;
    module->n_envelopes = n_envelopes;
    configuration_init (&module->configuration);
}

void module_deinit (module_t *module) {

    free (module->title);
    free (module->composer);
    free (module->description);
    free (module->channels);
    free (module->patterns);
    free (module->envelopes);

    /* TODO figure out how to do complete deinit */
}

void module_init_empty (module_t *module) {

    const size_t n_channels = 4;
    const size_t n_rows     = 32;

    size_t i;

    module_init (module,
                 "n00 s0ng",
                 "some1 kewl",
                 "kewlest song ever",
                 n_channels,
                 n_channels,
                 2);

    for (i = 0; i < n_channels; i++) {

        channel_init (&module->channels[i], 1);
        module->channels[i].i_patterns[0] = i;
        pattern_init (&module->patterns[i], n_rows);
    }

    envelope_init (&module->envelopes[0], 4, LOOP_MODE_NONE);
    module->envelopes[0].samples[0].amplitude = 0;
    module->envelopes[0].samples[1].amplitude = 1;
    module->envelopes[0].samples[2].amplitude = 0.5;
    module->envelopes[0].samples[3].amplitude = 0;
    module->envelopes[0].samples[0].duration = 0.025;
    module->envelopes[0].samples[1].duration = 0.1;
    module->envelopes[0].samples[2].duration = 0.5;
    module->envelopes[0].samples[3].duration = 0;

    envelope_init (&module->envelopes[1], 4, LOOP_MODE_NORMAL);
    module->envelopes[1].i_loop_begin = 2;
    module->envelopes[1].i_loop_end = 2;
    module->envelopes[1].samples[0].amplitude = 0;
    module->envelopes[1].samples[1].amplitude = 1;
    module->envelopes[1].samples[2].amplitude = 0.75;
    module->envelopes[1].samples[3].amplitude = 0;
    module->envelopes[1].samples[0].duration = 0.05;
    module->envelopes[1].samples[1].duration = 0.2;
    module->envelopes[1].samples[2].duration = 0.5;
    module->envelopes[1].samples[3].duration = 0;
}

int module_load (module_t *module, char *path) {

    /* TODO */

    return -1; /* failure....... */
}

int module_save (module_t *module, char *path) {

    /* TODO */

    return -1; /* failure....... */
}
