#ifndef ENVELOPE_GENERATOR_H
#define ENVELOPE_GENERATOR_H

#include <stdbool.h>

#include "envelope.h"

typedef struct envelope_generator_t {

    double phase;
    double output;
    bool sustain;

} envelope_generator_t;

void envelope_generator_init     (envelope_generator_t *envelope_generator);
void envelope_generator_process  (envelope_generator_t *envelope_generator,
                                  envelope_t *envelope,
                                  double delta_time);
void envelope_generator_note_on  (envelope_generator_t *envelope_generator);
void envelope_generator_note_off (envelope_generator_t *envelope_generator);

#endif /* ENVELOPE_GENERATOR_H */
