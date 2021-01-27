#ifndef SOUNDCHIP_H
#define SOUNDCHIP_H

#include <stdint.h>

#include "synth.h"

typedef struct soundchip_t {

    synth_t synth;

    /* 8 bit volume registers, 4 bits per output channel */
    uint8_t volume_registers[N_OPERATORS];

    /* 8 per 4 times 4 bit modulation index channels */
    uint8_t modulation_registers[N_OPERATORS];

    /* lower 8 bits of 12 bit period registers */
    uint8_t period_low_registers[N_OPERATORS];

    /* upper 4 bits of 12 bit period registers */
    uint8_t period_high_registers[N_OPERATORS / 2];

    /* filtered output */
    double outputs[N_OUTPUTS];

} soundchip_t;

void soundchip_init            (soundchip_t *soundchip);
void soundchip_deinit          (soundchip_t *soundchip);
void soundchip_flush_registers (soundchip_t *soundchip);
void soundchip_process         (soundchip_t *soundchip, double audio_rate);

#endif /* SOUNDCHIP_H */
