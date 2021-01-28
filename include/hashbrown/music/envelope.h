#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <stdlib.h>

typedef struct envelope_sample_t {

    double amplitude;
    double duration;

} envelope_sample_t;

typedef enum envelope_loop_mode_t {

    LOOP_MODE_NONE,
    LOOP_MODE_NORMAL,
    LOOP_MODE_PING_PONG,

    N_LOOP_MODE

} envelope_loop_mode_t;

typedef struct envelope_t {

    size_t n_samples;
    envelope_sample_t *samples;

    size_t i_loop_begin;
    size_t i_loop_end;

    envelope_loop_mode_t loop_mode;

} envelope_t;

void envelope_init   (envelope_t *envelope,
                      size_t n_samples,
                      envelope_loop_mode loop_mode);
void envelope_deinit (envelope_t *envelope);

#endif /* ENVELOPE_H */
