#ifndef NOISE_H
#define NOISE_H

#include <stdint.h>

typedef struct noise_generator_t {

    uint16_t state;
    double phase;
    double output;

} noise_generator_t;

void noise_generator_init    (noise_generator_t *noise_generator);
void noise_generator_shift   (noise_generator_t *noise_generator);
void noise_generator_process (noise_generator_t *noise_generator,
                              double frequency,
                              double audio_rate);

#endif /* NOISE_H */
