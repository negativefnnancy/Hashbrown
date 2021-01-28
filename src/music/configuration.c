#include <string.h>

#include <hashbrown/music/configuration.h>

void configuration_init (configuration_t *configuration) {

    memset (configuration, 0, sizeof (configuration_t));
}

