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
}
