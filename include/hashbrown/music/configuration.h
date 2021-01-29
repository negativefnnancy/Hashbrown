#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdint.h>

#include <hashbrown/sound/config.h>

typedef struct configuration_control_t {

    size_t i_channel;
    size_t i_envelope;
    double multiplier;

} configuration_control_t;

typedef struct configuration_t {

    uint8_t bitmasks[N_OUTPUTS];
    uint8_t flags;

    configuration_control_t volume[N_OPERATORS];
    configuration_control_t frequency[N_OPERATORS];
    configuration_control_t modulation[N_OPERATORS * 2];

    double tempo; /* bpm */
    unsigned int rows_per_beat;
    unsigned int ticks_per_row;

} configuration_t;

void configuration_init (configuration_t *configuration);

#endif /* CONFIGURATION_H */
