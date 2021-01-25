#ifndef OPERATOR_H
#define OPERATOR_H

#include <stdlib.h>

#include "config.h"
#include "modulator.h"
#include "oscillator.h"

typedef struct operator_t {

    double frequency;
    double amplitudes[N_OUTPUTS];

    size_t n_modulators;
    modulator_t *modulators;

    size_t n_ring_modulators;
    modulator_t *ring_modulators;

    oscillator_t oscillator;

} operator_t;

void operator_init    (operator_t *operator,
                       size_t n_modulators,
                       size_t n_ring_modulators);
void operator_deinit  (operator_t *operator);
void operator_process (operator_t *operator,
                       double outputs[N_OUTPUTS],
                       double audio_rate);

#endif /* OPERATOR_H */
