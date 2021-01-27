#include <string.h>

#include <hashbrown/sound/soundchip.h>

void soundchip_init (soundchip_t *soundchip) {

    memset (soundchip, 0, sizeof (soundchip_t));
    synth_init (&soundchip->synth);
}

void soundchip_deinit (soundchip_t *soundchip) {

    synth_deinit (&soundchip->synth);
}

void soundchip_flush_registers (soundchip_t *soundchip) {

    size_t i;

    /* update operator output volumes */
    for (i = 0; i < N_OPERATORS; i++) {
        uint8_t reg = soundchip->volume_registers[i];
        double left  = (reg & 0x0f) / (double) 0x100;
        double right = (reg & 0xf0 >> 4) / (double) 0x100;
        soundchip->synth.operators[i].amplitudes[0] = left;
        soundchip->synth.operators[i].amplitudes[1] = right;
    }

    /* update operator frequencies */
    for (i = 0; i < N_OPERATORS; i++) {
        uint8_t register_high = soundchip->period_high_registers[i];
        uint8_t register_low  = soundchip->period_low_registers[i];
        double period = (register_low | (register_high << 8)) / 4096.0;
        soundchip->synth.operators[i].frequency = 111861 / period;
    }

    /* update modulation indices */
}

void soundchip_process (soundchip_t *soundchip, double audio_rate) {

    /* TODO: internal vs external sampling rate */
    /* TODO: simulation of external audio filtering */

    synth_process (&soundchip->synth, audio_rate);

    memcpy (soundchip->outputs,
            soundchip->synth.outputs,
            sizeof (soundchip->outputs));
}
