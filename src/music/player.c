#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hashbrown/music/player.h>

void player_init (player_t *player,
                  module_t *module,
                  soundchip_t *soundchip) {

    memset (player, 0, sizeof (player_t));
    player->module = module;
    player->soundchip = soundchip;
    player->configuration = module->configuration;
    player->positions
        = calloc (module->n_channels, sizeof (channel_position_t));
    player->controllers
        = calloc (module->n_channels, sizeof (channel_controller_t));
}

void player_deinit (player_t *player) {

    free (player->positions);
}

pattern_t *get_pattern (player_t *player, size_t i_channel) {

    channel_t *channel = &player->module->channels[i_channel];
    size_t i_i_pattern = player->positions[i_channel].i_i_pattern;
    size_t i_pattern = channel->i_patterns[i_i_pattern];
    return &player->module->patterns[i_pattern];
}

void process_tick (player_t *player, double tick_duration) {

    size_t i;
    bool first_tick = player->tick % player->configuration.ticks_per_row == 0;

    /*  update channel controllers based on pattern rows for this tick */
    for (i = 0; i < player->module->n_channels; i++) {

        pattern_t *pattern = get_pattern (player, i);
        row_t row = pattern->rows[player->positions[i].i_row];

        /* update for the row onset */
        if (first_tick) {

            /* set note frequency */
            int note = row.note & 0x7f;
            if (note)
                player->controllers[i].frequency
                    = 440.0 * pow (2, (note - 69) / 12.0);

            /* set gate if gate bit is set */
            if (row.note & 0x80)
                player->controllers[i].gate = row.note == 0x80
                                            ? CHANNEL_GATE_NOTE_OFF
                                            : CHANNEL_GATE_NOTE_ON;

            /* set volume if gate bit is set
             * or if volume is non zero */
            if (row.volume > 0 || row.note & 0x80) {

                player->controllers[i].volume_left
                    = ((row.volume & 0x0f) >> 0) / (double) 0xf;
                player->controllers[i].volume_right
                    = ((row.volume & 0xf0) >> 4) / (double) 0xf;
            }

            /* TODO effects */

        } else {

            player->controllers[i].gate = CHANNEL_GATE_NULL;

            /* TODO effects */
        }
    }

    /* TODO: unify the following loops */

    /* update volume envelope generators and chip registers */
    for (i = 0; i < N_OPERATORS; i++) {

        double volume_modulation;

        /* the configuration for this control */
        configuration_control_t control = player->configuration.volume[i];

        /* figure out which channel is controlling this operator */
        size_t i_channel = control.i_channel;

        /* get the controller for this channel */
        channel_controller_t controller = player->controllers[i_channel];

        /* update envelope generator controls */
        if (controller.gate) {

            if (controller.gate == CHANNEL_GATE_NOTE_ON)
                envelope_generator_note_on (&player->volume_generators[i]);

            else if (controller.gate == CHANNEL_GATE_NOTE_OFF)
                envelope_generator_note_off (&player->volume_generators[i]);
        }

        /* the value for all controls is going to be
         * the basic input value from the row data
         * modulated by the envelope generator
         * times the multiplier */
        volume_modulation = player->volume_generators[i].output
                          * player->configuration.volume[i].multiplier;

        /* set the volume register */
        soundchip_set_volume (player->soundchip,
                              i,
                              controller.volume_left  * volume_modulation,
                              controller.volume_right * volume_modulation);

        /* update the envelope generator */
        envelope_generator_process (&player->volume_generators[i],
                                    &player->module->envelopes[control.i_envelope],
                                    tick_duration);
    }

    /* update frequency envelope generators and chip registers */
    for (i = 0; i < N_OPERATORS; i++) {

        double frequency_modulation;

        /* the configuration for this control */
        configuration_control_t control = player->configuration.frequency[i];

        /* figure out which channel is controlling this operator */
        size_t i_channel = control.i_channel;

        /* get the controller for this channel */
        channel_controller_t controller = player->controllers[i_channel];

        /* update envelope generator controls */
        if (controller.gate) {

            if (controller.gate == CHANNEL_GATE_NOTE_ON)
                envelope_generator_note_on (&player->frequency_generators[i]);

            else if (controller.gate == CHANNEL_GATE_NOTE_OFF)
                envelope_generator_note_off (&player->frequency_generators[i]);
        }

        /* the value for all controls is going to be
         * the basic input value from the row data
         * modulated by the envelope generator
         * times the multiplier */
        frequency_modulation = player->frequency_generators[i].output
                             * player->configuration.frequency[i].multiplier;

        /* set the frequency register */
        soundchip_set_frequency (player->soundchip,
                                 i,
                                 controller.frequency
                                 * pow (2, frequency_modulation));

        /* update the envelope generator */
        envelope_generator_process (&player->frequency_generators[i],
                                    &player->module->envelopes[control.i_envelope],
                                    tick_duration);
    }

    /* update modulation envelope generators and chip registers */
    for (i = 0; i < N_OPERATORS * 2; i++) {

        double modulation_modulation;

        /* the configuration for this control */
        configuration_control_t control = player->configuration.modulation[i];

        /* figure out which channel is controlling this operator */
        size_t i_channel = control.i_channel;

        /* get the controller for this channel */
        channel_controller_t controller = player->controllers[i_channel];

        /* update envelope generator controls */
        if (controller.gate) {

            if (controller.gate == CHANNEL_GATE_NOTE_ON)
                envelope_generator_note_on (&player->modulation_generators[i]);

            else if (controller.gate == CHANNEL_GATE_NOTE_OFF)
                envelope_generator_note_off (&player->modulation_generators[i]);
        }

        /* the value for all controls is going to be
         * the basic input value from the row data
         * modulated by the envelope generator
         * times the multiplier */
        modulation_modulation = player->modulation_generators[i].output
                              * player->configuration.modulation[i].multiplier;

        /* set the modulation register */
        soundchip_set_modulation (player->soundchip,
                                 i,
                                 modulation_modulation);

        /* update the envelope generator */
        envelope_generator_process (&player->modulation_generators[i],
                                    &player->module->envelopes[control.i_envelope],
                                    tick_duration);
    }

    /* set chip output bitmask registers */
    memcpy (player->soundchip->bitmasks,
            player->configuration.bitmasks,
            sizeof (player->soundchip->bitmasks));

    /* set chip flags register */
    player->soundchip->flags_register = player->configuration.flags;

    /* update chip state */
    soundchip_flush_registers (player->soundchip);
}

void player_process (player_t *player, double audio_rate) {

    double tick_duration = 60.0 / (player->configuration.tempo
                                   * player->configuration.rows_per_beat
                                   * player->configuration.ticks_per_row);

    /* tick timer */
    while (player->timer <= 0) {

        /* row timer */
        while (player->tick >= player->configuration.ticks_per_row) {

            size_t i;
            for (i = 0; i < player->module->n_channels; i++) {
                
                pattern_t *pattern;
                size_t *i_row = &player->positions[i].i_row;
                (*i_row)++;

                while (*i_row >= (pattern = get_pattern (player, i))->n_rows) {

                    channel_t *channel;
                    size_t *i_i_pattern = &player->positions[i].i_i_pattern;
                    (*i_i_pattern)++;
                    *i_row -= pattern->n_rows;

                    while (*i_i_pattern
                            >= (channel = &player->module->channels[i])
                               ->n_patterns) {

                        /* TODO song loop points */
                        *i_i_pattern -= channel->n_patterns;
                    }
                }
            }

            player->tick = 0;
        }

        /* update soundchip for this tick */
        process_tick (player, tick_duration);

        player->tick++;
        player->timer += tick_duration;
    }
    player->timer -= 1 / audio_rate;

    /* audio rate */
    soundchip_process (player->soundchip, audio_rate);
    memcpy (player->outputs, player->soundchip->outputs, SIZE_FRAME);
}
