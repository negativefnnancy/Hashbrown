#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "module.h"
#include "configuration.h"

typedef struct playback_context_t {

    /* the module being played */
    module_t module;

    /* live configuration */
    configuration_t configuration;

} playback_t;

#endif /* PLAYBACK_H */
