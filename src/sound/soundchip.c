#include <hashbrown/sound/soundchip.h>

void soundchip_init (soundchip_t *soundchip) {

    synth_init (&soundchip->synth);
}

void soundchip_deinit (soundchip_t *soundchip) {

    synth_deinit (&soundchip->synth);
}

void soundchip_update_synth (soundchip_t *soundchip) {

    /* TODO: copy registers to synth parameters */
}

void soundchip_process (soundchip_t *soundchip, double audio_rate) {

    sonudchip_update_synth (soundchip);

    synth_process (&soundchip->synth, audio_rate);
}
