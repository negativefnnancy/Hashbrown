#ifndef SYNTH_H
#define SYNTH_H

#include "config.h"
#include "operator.h"

typedef struct synth_t {

    operator_t operators[N_OPERATORS];
    double outputs[N_OUTPUTS];

} synth_t;

void synth_init    (synth_t *synth);
void synth_deinit  (synth_t *synth);
void synth_process (synth_t *synth, double audio_rate);

#endif /* SYNTH_H */
