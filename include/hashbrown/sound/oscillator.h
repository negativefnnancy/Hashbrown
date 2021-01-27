#ifndef OSCILLATOR_H
#define OSCILLATOR_H

typedef struct oscillator_t {

    double phase;
    double output;

} oscillator_t;

void oscillator_init    (oscillator_t *oscillator);
void oscillator_process (oscillator_t *oscillator,
                         double frequency,
                         double phase,
                         double audio_rate);

#endif /* OSCILLATOR_H */
