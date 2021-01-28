#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdint.h>

#include <hashbrown/sound/config.h>

#define N_CONTROLS (N_OPERATORS * 4)

typedef struct configuration_t {

    uint8_t bitmasks[N_OUTPUTS];
    uint8_t flags;

    struct {
       
        size_t i_channel;
        size_t i_envelope;
        double multiplier;

    } controls[N_CONTROLS];

    double tempo; /* bpm */
    unsigned int rows_per_beat;
    unsigned int ticks_per_row;

} configuration_t;

#endif /* CONFIGURATION_H */
