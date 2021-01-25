#include <string.h>

#include <hashbrown/sound/synth.h>

void synth_init (synth_t *synth) {

    size_t i;

    for (i = 0; i < N_OPERATORS; i++) {
        operator_init (&synth->operators[i], 1);
        synth->operators[i].modulators[0].operator
            = &synth->operators[(i + 1) % N_OPERATORS];
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
