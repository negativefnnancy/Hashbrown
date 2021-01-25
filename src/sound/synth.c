#include <string.h>

#include <hashbrown/sound/synth.h>

void synth_init (synth_t *synth) {

    /* TODO: figure out a more elegant way to initialize the particular
     * configuration of the frequency modulation and ring modulation paths */

    size_t i;

    for (i = 0; i < N_OPERATORS; i++) {
        operator_init (&synth->operators[i], 3, 1);
        synth->operators[i].n_modulators = 1;
        synth->operators[i].n_ring_modulators = 0;
        synth->operators[i].modulators[0].operator
            = &synth->operators[(i + 1) % N_OPERATORS];
    }

    for (i = 0; i < N_OPERATORS; i += 4) {

        synth->operators[i].n_modulators = 3;

        /* second and third modulators */
        synth->operators[i].modulators[1].operator
            = &synth->operators[i + 2];
        synth->operators[i].modulators[2].operator
            = &synth->operators[i + 3];

        /* feedback path */
        synth->operators[i + 3].n_modulators = 2;
        synth->operators[i + 3].modulators[1].operator
            = &synth->operators[i + 3];

        /* ring modulators */
        synth->operators[(i + 4) % N_OPERATORS].n_ring_modulators = 1;
        synth->operators[(i + 4) % N_OPERATORS].ring_modulators[0].operator
            = &synth->operators[i + 3];
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
