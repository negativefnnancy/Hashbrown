#ifndef SOUNDCHIP_H
#define SOUNDCHIP_H

#include <stdint.h>

#include "synth.h"

#define MAX_PERIOD 111861
#define MAX_MODULATION_INDEX 3.1415926535897
#define MAX_FEEDBACK 0.5
#define MAX_RING_MODULATION 1
#define OUTPUT_CLOCK_RATE 22050

typedef struct soundchip_t {

    synth_t synth;

    /* 8 bit volume registers, 4 bits per output channel */
    uint8_t volume_registers[N_OPERATORS];

    /* 8 per 4 operators times 8 bit modulation index channels */
    uint8_t modulation_registers[N_OPERATORS * 2];

    /* lower 8 bits of 12 bit period registers */
    uint8_t period_low_registers[N_OPERATORS];

    /* upper 4 bits of 12 bit period registers */
    uint8_t period_high_registers[N_OPERATORS / 2];

    /* misc flags register */
    uint8_t flags_register;

    /* output bitmasks */
    uint8_t bitmasks[N_OUTPUTS];

    /* filtered output */
    double outputs[N_OUTPUTS];

    /* the sampling timer */
    double timer;

} soundchip_t;

void soundchip_init            (soundchip_t *soundchip);
void soundchip_deinit          (soundchip_t *soundchip);
void soundchip_flush_registers (soundchip_t *soundchip);
void soundchip_process         (soundchip_t *soundchip, double audio_rate);

void soundchip_set_frequency (soundchip_t *soundchip,
                              size_t i_operator,
                              double frequency);

void soundchip_set_volume (soundchip_t *soundchip,
                           size_t i_operator,
                           double left,
                           double right);

void soundchip_set_modulation (soundchip_t *soundchip,
                               size_t i_modulation_index,
                               double modulation_index);

#endif /* SOUNDCHIP_H */
