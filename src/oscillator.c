#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "oscillator.h"
#include "util.h"

void oscillator_init (oscillator_t *oscillator) {

    memset (oscillator, 0, sizeof (oscillator_t));
}

void oscillator_process (oscillator_t *oscillator,
                         double frequency,
                         double audio_rate) {

    oscillator->output = sin (oscillator->phase);
    oscillator->phase += 2 * M_PI * frequency / audio_rate;
}
