#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hashbrown/sound/oscillator.h>

#include "util.h"

void oscillator_init (oscillator_t *oscillator) {

    memset (oscillator, 0, sizeof (oscillator_t));
}

void oscillator_process (oscillator_t *oscillator,
                         double frequency,
                         double audio_rate) {

    oscillator->output = sin (2 * M_PI * oscillator->phase);
    oscillator->phase += frequency / audio_rate;
}
