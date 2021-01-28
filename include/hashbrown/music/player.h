#ifndef PLAYER_H
#define PLAYER_H

#include <hashbrown/sound/soundchip.h>

#include "module.h"
#include "configuration.h"

typedef struct player_t {

    /* the module being played */
    module_t *module;

    /* the chip being played */
    soundchip_t *soundchip;

    /* live configuration */
    configuration_t configuration;

    /* stored audio output */
    double outputs[N_OUTPUTS];

} player_t;

void player_init    (player_t *player,
                     module_t *module,
                     soundchip_t *soundchip);
void player_process (player_t *player, double audio_rate);

#endif /* PLAYER_H */
