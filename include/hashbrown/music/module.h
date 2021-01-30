#ifndef MODULE_H
#define MODULE_H

#include "channel.h"
#include "pattern.h"
#include "envelope.h"
#include "configuration.h"

#define MAX_TITLE_LENGTH 64
#define MAX_COMPOSER_LENGTH 64
#define MAX_DESCRIPTION_LENGTH 256

typedef struct module_t {

    char *title;
    char *composer;
    char *description;

    size_t n_channels;
    channel_t *channels;

    size_t n_patterns;
    pattern_t *patterns;

    size_t n_envelopes;
    envelope_t *envelopes;

    /* initial configuration */
    configuration_t configuration;

} module_t;

void module_init       (module_t *module,
                        char *title,
                        char *composer,
                        char *description,
                        size_t n_channels,
                        size_t n_patterns,
                        size_t n_envelopes);
void module_deinit     (module_t *module);
void module_init_empty (module_t *module);
int module_load        (module_t *module, char *path);
int module_save        (module_t *module, char *path);

#endif /* MODULE_H */
