#ifndef SOUNDCHIP_H
#define SOUNDCHIP_H

#include "synth.h"

typedef struct soundchip_t {

    synth_t synth;

    /* TODO register abi design */

} soundchip_t;

void soundchip_init    (soundchip_t *soundchip);
void soundchip_deinit  (soundchip_t *soundchip);
void soundchip_process (soundchip_t *soundchip, double audio_rate);

#endif /* SOUNDCHIP_H */
