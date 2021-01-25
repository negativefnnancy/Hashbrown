#include <stdlib.h>
#include <string.h>

#include <hashbrown/sound/operator.h>

void operator_init (operator_t *operator, size_t n_modulators) {

    memset (operator, 0, sizeof (operator_t));
    operator->n_modulators = n_modulators;
    operator->modulators = calloc (n_modulators, sizeof (modulator_t));
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

    for (i = 0; i < operator->n_modulators; i++)
        modulator_process (&operator->modulators[i]);

    for (i = 0; i < operator->n_modulators; i++)
        modulator_sum += operator->modulators[i].output;

    frequency = operator->frequency + operator->frequency * modulator_sum;
    oscillator_process (&operator->oscillator, frequency, audio_rate);

    for (i = 0; i < N_OUTPUTS; i++)
        outputs[i] += operator->oscillator.output * operator->amplitudes[i];
}
