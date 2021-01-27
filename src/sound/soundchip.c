#include <string.h>

#include <hashbrown/sound/soundchip.h>

void soundchip_init (soundchip_t *soundchip) {

    memset (soundchip, 0, sizeof (soundchip_t));
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

        uint8_t reg = soundchip->modulation_registers[i];
        double modulation_index = get_low_nibble (reg) * MAX_MODULATION_INDEX;
        soundchip->synth.operators[i].modulators[0].modulation_index
            = modulation_index;
    }

    /* update extra chain of modulation indices */
    for (i = 0; i < N_OPERATORS; i += 4) {

        uint8_t reg_1 = soundchip->modulation_registers[i + 0];
        uint8_t reg_2 = soundchip->modulation_registers[i + 1];
        uint8_t reg_3 = soundchip->modulation_registers[i + 2];
        uint8_t reg_4 = soundchip->modulation_registers[i + 3];
        double modulation_index_1 = get_high_nibble (reg_1) * MAX_MODULATION_INDEX;
        double modulation_index_2 = get_high_nibble (reg_2) * MAX_MODULATION_INDEX;
        double modulation_index_3 = get_high_nibble (reg_3) * MAX_MODULATION_INDEX;
        double modulation_index_4 = get_high_nibble (reg_4) * MAX_MODULATION_INDEX;
        soundchip->synth.operators[i].modulators[1].modulation_index
            = modulation_index_1;
        soundchip->synth.operators[i].modulators[2].modulation_index
            = modulation_index_2;
        soundchip->synth.operators[i + 3].modulators[1].modulation_index
            = modulation_index_3;
        soundchip->synth.operators[i].ring_modulators[0].modulation_index
            = modulation_index_4;
    }
}

void soundchip_process (soundchip_t *soundchip, double audio_rate) {

    /* TODO: internal vs external sampling rate */
    /* TODO: simulation of external audio filtering */

    synth_process (&soundchip->synth, audio_rate);

    memcpy (soundchip->outputs, soundchip->synth.outputs, SIZE_FRAME);
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

    size_t i_register = i_modulation_index % N_OPERATORS;
    uint8_t value = modulation_index * 0xf;
    uint8_t reg = soundchip->modulation_registers[i_register];
    if (i_modulation_index / N_OPERATORS == 0)
        soundchip->modulation_registers[i_register]
            = reg & 0xf0 | (value << 0) & 0x0f;
    else
        soundchip->modulation_registers[i_register]
            = reg & 0x0f | (value << 4) & 0xf0;
}

