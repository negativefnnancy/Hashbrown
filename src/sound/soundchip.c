#include <string.h>

#include <hashbrown/sound/soundchip.h>

void soundchip_init (soundchip_t *soundchip) {

    memset (soundchip, 0, sizeof (soundchip_t));
    memset (soundchip->bitmasks, 0xff, sizeof (soundchip->bitmasks));
    synth_init (&soundchip->synth);
}

void soundchip_deinit (soundchip_t *soundchip) {

    synth_deinit (&soundchip->synth);
}

double get_high_nibble (uint8_t reg) {

    return ((reg & 0xf0) >> 4) / (double) 0xf;
}

double get_low_nibble (uint8_t reg) {

    return ((reg & 0x0f) >> 0) / (double) 0xf;
}

void soundchip_flush_registers (soundchip_t *soundchip) {

    size_t i;

    /* update operator output volumes */
    for (i = 0; i < N_OPERATORS; i++) {
        uint8_t reg = soundchip->volume_registers[i];
        double left  = get_low_nibble (reg);
        double right = get_high_nibble (reg);
        soundchip->synth.operators[i].amplitudes[0] = left;
        soundchip->synth.operators[i].amplitudes[1] = right;
    }

    /* update operator frequencies */
    for (i = 0; i < N_OPERATORS; i++) {

        uint8_t register_high, register_low;
        double period;
        size_t i_high_register = i / 2;

        if (i % 2 == 0)
            register_high
                = (soundchip->period_high_registers[i_high_register] & 0x0f)
                >> 0;
        else
            register_high
                = (soundchip->period_high_registers[i_high_register] & 0xf0)
                >> 4;

        register_low  = soundchip->period_low_registers[i];
        period = (register_low | (register_high << 8));
        soundchip->synth.operators[i].frequency = MAX_PERIOD / period;
    }

    /* update first chain of modulation indices */
    for (i = 0; i < N_OPERATORS; i++) {

        double modulation_index = (double) soundchip->modulation_registers[i]
                                * MAX_MODULATION_INDEX
                                / 0xff;
        soundchip->synth.operators[i].modulators[0].modulation_index
            = modulation_index;
    }

    /* update extra chain of modulation indices */
    for (i = 0; i < N_OPERATORS; i += 4) {

        uint8_t reg_1 = soundchip->modulation_registers[N_OPERATORS + i + 0] & 0xff;
        uint8_t reg_2 = soundchip->modulation_registers[N_OPERATORS + i + 1] & 0xff;
        uint8_t reg_3 = soundchip->modulation_registers[N_OPERATORS + i + 2] & 0xff;
        uint8_t reg_4 = soundchip->modulation_registers[N_OPERATORS + i + 3] & 0xff;
        double modulation_index_1 = (double) reg_1 * MAX_MODULATION_INDEX / 0xff;
        double modulation_index_2 = (double) reg_2 * MAX_MODULATION_INDEX / 0xff;
        double modulation_index_3 = (double) reg_3 * MAX_FEEDBACK         / 0xff;
        double modulation_index_4 = (double) reg_4 * MAX_RING_MODULATION  / 0xff;
        soundchip->synth.operators[i].modulators[1].modulation_index
            = modulation_index_1;
        soundchip->synth.operators[i].modulators[2].modulation_index
            = modulation_index_2;
        soundchip->synth.operators[i + 3].modulators[1].modulation_index
            = modulation_index_3;
        soundchip->synth.operators[i].ring_modulators[0].modulation_index
            = modulation_index_4;
    }

    /* update flags */
    soundchip->synth.operators[N_OPERATORS - 2].noise_mode
        = soundchip->flags_register & 0x01;
}

void soundchip_process (soundchip_t *soundchip, double audio_rate) {

    size_t i;

    /* TODO: internal vs external sampling rate */
    /* TODO: simulation of external audio filtering */

    /* update the state of the internal synthesizer */
    synth_process (&soundchip->synth, audio_rate);

    /* sample the synth at 8 bits */
    for (i = 0; i < N_OUTPUTS; i++) {

        double clipped = soundchip->synth.outputs[i];
        if (clipped > 1)
            clipped = 1;
        else if (clipped < -1)
            clipped = -1;
        uint8_t sampled = (clipped + 1) / 2 * 255.0;
        uint8_t masked = sampled & soundchip->bitmasks[i];
        soundchip->outputs[i] = masked / 255.0 * 2 - 1;
    }
}

void soundchip_set_frequency (soundchip_t *soundchip,
                              size_t i_operator,
                              double frequency) {

    size_t i_high_register = i_operator / 2;
    uint8_t high_register = soundchip->period_high_registers[i_high_register];
    uint16_t period = MAX_PERIOD / frequency;
    if (i_operator % 2 == 0)
        soundchip->period_high_registers[i_high_register]
            = high_register & 0xf0 | (period >> 8) & 0x0f;
    else
        soundchip->period_high_registers[i_high_register]
            = high_register & 0x0f | (period >> 4) & 0xf0;
    soundchip->period_low_registers[i_operator] = (int) period & 0xff;
}

void soundchip_set_volume (soundchip_t *soundchip,
                           size_t i_operator,
                           double left,
                           double right) {

    uint8_t volume = (((int) (left  * 0xf) << 0) & 0x0f)
                   | (((int) (right * 0xf) << 4) & 0xf0);
    soundchip->volume_registers[i_operator] = volume;
}

void soundchip_set_modulation (soundchip_t *soundchip,
                               size_t i_modulation_index,
                               double modulation_index) {

    soundchip->modulation_registers[i_modulation_index]
        = modulation_index * 0xff;
}

