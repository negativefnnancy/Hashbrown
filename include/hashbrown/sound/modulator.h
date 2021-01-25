#ifndef MODULATOR_H
#define MODULATOR_H

struct operator_t;

typedef struct modulator_t {

    double modulation_index;
    struct operator_t *operator;

    double output;

} modulator_t;

void modulator_process (modulator_t *modulator);

#endif /* MODULATOR_H */
