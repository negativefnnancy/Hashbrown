#include <stdlib.h>

#include <hashbrown/music/player.h>

void player_init (player_t *player,
                  module_t *module,
                  soundchip_t *soundchip) {

    player->module = module;
    player->soundchip = soundchip;
    player->configuration = module->configuration;
}

void player_process (player_t *player, double audio_rate) {

    /* control rate */
    /* TODO */

    /* audio rate */
    soundchip_process (player->soundchip, audio_rate);
    memcpy (player->outputs, player->soundchip->outputs, SIZE_FRAME);
}
