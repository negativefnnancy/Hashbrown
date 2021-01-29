#ifndef PLAYER_H
#define PLAYER_H

#include <hashbrown/sound/soundchip.h>

#include "module.h"
#include "configuration.h"
#include "envelope_generator.h"

typedef struct channel_position_t {

    size_t i_i_pattern;
    size_t i_row;

} channel_position_t;

typedef enum channel_gate_t {

    CHANNEL_GATE_NULL,
    CHANNEL_GATE_NOTE_ON,
    CHANNEL_GATE_NOTE_OFF,

    N_CHANNEL_GATES

} channel_gate_t;

typedef struct channel_controller_t {

    double volume_left;
    double volume_right;
    double frequency;
    channel_gate_t gate;

} channel_controller_t;

typedef struct player_t {

    /* the module being played */
    module_t *module;

    /* the chip being played */
    soundchip_t *soundchip;

    /* live configuration */
    configuration_t configuration;

    /* stored audio output */
    double outputs[N_OUTPUTS];

    /* tick timer */
    double timer;
    unsigned int tick;

    /* channel positions */
    channel_position_t *positions;

    /* the state of the controllers controlled by each channel */
    channel_controller_t *controllers;

    /* envelope generators for every control */
    envelope_generator_t volume_generators[N_OPERATORS];
    envelope_generator_t frequency_generators[N_OPERATORS];
    envelope_generator_t modulation_generators[N_OPERATORS * 2];

} player_t;

void player_init    (player_t *player,
                     module_t *module,
                     soundchip_t *soundchip);
void player_deinit  (player_t *player);
void player_process (player_t *player, double audio_rate);

#endif /* PLAYER_H */
