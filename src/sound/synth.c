#include <string.h>

#include <hashbrown/sound/synth.h>

void synth_init (synth_t *synth) {

    size_t i;

    for (i = 0; i < N_OPERATORS; i++) {

        /* determine how many modulators this operator has */
        size_t n_modulators = 1;

        /* every 4 operators form a modulation chain */
        if (i % 4 == 0)
            n_modulators++;

        /* initialize the operator with the determined number of modulators */
        operator_init (&synth->operators[i], n_modulators, 1);

        /* the ring modulator */
        synth->operators[i].ring_modulators[0].operator
            = &synth->operators[(i + 2) % N_OPERATORS];

        /* the first modulator */
        synth->operators[i].modulators[0].operator
            = &synth->operators[(i + 1) % N_OPERATORS];

        if (i % 4 == 0)
            synth->operators[i].modulators[1].operator
                = &synth->operators[(i + 4) % N_OPERATORS];
    }
}

void synth_deinit (synth_t *synth) {

    size_t i;

    for (i = 0; i < N_OPERATORS; i++)
        operator_deinit (&synth->operators[i]);
}

void synth_process (synth_t *synth, double audio_rate) {

    size_t i;

    memset (synth->outputs, 0, sizeof (synth->outputs));

    for (i = 0; i < N_OPERATORS; i++)
        operator_process (&synth->operators[i],
                          synth->outputs,
                          audio_rate);
}
