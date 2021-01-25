#include <stdlib.h>
#include <string.h>

#include <hashbrown/sound/operator.h>

void operator_init (operator_t *operator,
                    size_t n_modulators,
                    size_t n_ring_modulators) {

    memset (operator, 0, sizeof (operator_t));
    operator->n_modulators = n_modulators;
    operator->modulators = calloc (n_modulators, sizeof (modulator_t));
    operator->n_ring_modulators = n_ring_modulators;
    operator->ring_modulators
        = calloc (n_ring_modulators, sizeof (modulator_t));
    oscillator_init (&operator->oscillator);
}

void operator_deinit (operator_t *operator) {

    free (operator->modulators);
}

void operator_process (operator_t *operator,
                       double outputs[N_OUTPUTS],
                       double audio_rate) {

    size_t i;
    double frequency;
    double modulator_sum = 0;
    double ring_modulator_product = 1;
    double output;

    for (i = 0; i < operator->n_modulators; i++) {
        modulator_process (&operator->modulators[i]);
        modulator_sum += operator->modulators[i].output;
    }

    for (i = 0; i < operator->n_ring_modulators; i++) {
        modulator_process (&operator->ring_modulators[i]);
        ring_modulator_product
            *= 1 + (operator->ring_modulators[i].output - 1)
            * operator->ring_modulators[i].modulation_index;
    }

    frequency = operator->frequency + operator->frequency * modulator_sum;
    oscillator_process (&operator->oscillator, frequency, audio_rate);
    output = operator->oscillator.output * ring_modulator_product;

    for (i = 0; i < N_OUTPUTS; i++)
        outputs[i] += output * operator->amplitudes[i];
}
