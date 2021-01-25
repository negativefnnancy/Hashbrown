#include <hashbrown/sound/soundchip.h>

void soundchip_init (soundchip_t *soundchip) {

    synth_init (&soundchip->synth);
}

void soundchip_deinit (soundchip_t *soundchip) {

    synth_deinit (&soundchip->synth);
}

void soundchip_process (soundchip_t *soundchip, double audio_rate) {

    /* TODO: copy registers to synth parameters */

    synth_process (&soundchip->synth, audio_rate);
}
