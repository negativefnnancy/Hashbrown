#ifndef SINE_H
#define SINE_H

#include <stdint.h>

#define N_SINE_SAMPLES 1024

extern const uint8_t sine_table[N_SINE_SAMPLES];

double sine_lookup (double phase);

#endif /* SINE_H */
