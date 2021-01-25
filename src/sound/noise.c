#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <hashbrown/sound/noise.h>

void noise_generator_init (noise_generator_t *noise_generator) {

    memset (noise_generator, 0, sizeof (noise_generator_t));
    noise_generator->state = 1;
}

void noise_generator_shift (noise_generator_t *noise_generator) {

    uint8_t feedback
        = (noise_generator->state & 0x0001)
        ^ (noise_generator->state & 0x0002) >> 1;

    noise_generator->state >>= 1;
    noise_generator->state |= feedback << 14;
}

void noise_generator_process (noise_generator_t *noise_generator,
                              double frequency,
                              double audio_rate) {

    noise_generator->output = noise_generator->state & 0x0001 ? 1 : -1;
    noise_generator->phase += frequency / audio_rate;
    while (noise_generator->phase >= 1) {
        noise_generator_shift (noise_generator);
        noise_generator->phase -= 1;
    }
}
