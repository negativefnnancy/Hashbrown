#ifndef OSCILLATOR_H
#define OSCILLATOR_H

typedef struct oscillator_t {

    double output;
    double phase;

} oscillator_t;

void oscillator_init    (oscillator_t *oscillator);
void oscillator_process (oscillator_t *oscillator,
                         double frequency,
                         double audio_rate);

#endif /* OSCILLATOR_H */
