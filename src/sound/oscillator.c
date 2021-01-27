#include <stdlib.h>
#include <string.h>

#include <hashbrown/sound/oscillator.h>
#include <hashbrown/sound/sine.h>

#include "util.h"

void oscillator_init (oscillator_t *oscillator) {

    memset (oscillator, 0, sizeof (oscillator_t));
}

void oscillator_process (oscillator_t *oscillator,
                         double frequency,
                         double phase,
                         double audio_rate) {

    oscillator->output = sine_lookup (2 * M_PI * (oscillator->phase + phase));
    oscillator->phase += frequency / audio_rate;
}
