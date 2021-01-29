/* music test
 *
 * this is meant to be a low level test of the music driver!!!!
 */

#include <hashbrown/sound/soundchip.h>
#include <hashbrown/music/module.h>
#include <hashbrown/music/player.h>

#include "audio.h"

#define AUDIO_RATE 48000
#define DURATION 10.666

void render_audio (player_t *player,
                   double audio_rate,
                   size_t n_frames,
                   double *buffer) {

    size_t i;
    for (i = 0; i < n_frames; i++) {

        player_process (player, audio_rate);
        memcpy (buffer, player->outputs, SIZE_FRAME);
        buffer += N_OUTPUTS;
    }
}

void populate_module (module_t *module) {

    /* some test sequences */
    pattern_init (&module->patterns[0], 32);
    row_init (&module->patterns[0].rows[0],  0xa8, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[1],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[2],  0xb4, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[3],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[4],  0xaa, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[5],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[6],  0xb6, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[7],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[8],  0xa7, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[9],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[10], 0xb3, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[11], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[12], 0xac, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[13], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[14], 0xb8, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[15], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[16], 0xa5, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[17], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[18], 0xb1, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[19], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[20], 0xaa, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[21], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[22], 0xb6, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[23], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[24], 0xa3, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[25], 0xaf, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[26], 0xa3, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[27], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[28], 0xa5, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[29], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[0].rows[30], 0xa7, 0xff, 0x0000);
    row_init (&module->patterns[0].rows[31], 0x00, 0x00, 0x0000);

    pattern_init (&module->patterns[1], 32);
    row_init (&module->patterns[1].rows[0],  0xc2, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[1],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[1].rows[2],  0xc4, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[3],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[1].rows[4],  0xbe, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[5],  0xbf, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[6],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[1].rows[7],  0xbd, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[8],  0xbe, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[9],  0xbd, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[10], 0xbb, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[11], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[1].rows[12], 0xbb, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[13], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[1].rows[14], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[15], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[1].rows[16], 0xbe, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[17], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[1].rows[18], 0xbe, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[19], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[20], 0xbb, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[21], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[22], 0xbf, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[23], 0xc2, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[24], 0xc4, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[25], 0xbf, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[26], 0xc2, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[27], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[28], 0xbf, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[29], 0xbb, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[30], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[1].rows[31], 0xbb, 0xff, 0x0000);

    pattern_init (&module->patterns[2], 32);
    row_init (&module->patterns[2].rows[0],  0xbf, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[1],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[2].rows[2],  0xc2, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[3],  0x00, 0x00, 0x0000);
    row_init (&module->patterns[2].rows[4],  0xc4, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[5],  0xbf, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[6],  0xc2, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[7],  0xbd, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[8],  0xbf, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[9],  0xbb, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[10], 0xbe, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[11], 0xbf, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[12], 0xbe, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[13], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[14], 0xbb, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[15], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[16], 0xbe, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[17], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[2].rows[18], 0xbb, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[19], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[20], 0xbf, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[21], 0xc2, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[22], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[23], 0xbe, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[24], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[25], 0xbb, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[26], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[27], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[2].rows[28], 0xbb, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[29], 0x00, 0x00, 0x0000);
    row_init (&module->patterns[2].rows[30], 0xbd, 0xff, 0x0000);
    row_init (&module->patterns[2].rows[31], 0x00, 0x00, 0x0000);

    /* sequence the patterns in the channels */
    channel_init (&module->channels[0], 2);
    module->channels[0].i_patterns[0] = 0;
    module->channels[0].i_patterns[1] = 0;
    channel_init (&module->channels[1], 2);
    module->channels[1].i_patterns[0] = 1;
    module->channels[1].i_patterns[1] = 2;

    /* define the envelopes */
    envelope_init (&module->envelopes[0], 4, LOOP_MODE_NONE);
    module->envelopes[0].samples[0].amplitude = 0;
    module->envelopes[0].samples[0].duration = 0.025;
    module->envelopes[0].samples[1].amplitude = 1;
    module->envelopes[0].samples[1].duration = 0.1;
    module->envelopes[0].samples[2].amplitude = 0.5;
    module->envelopes[0].samples[2].duration = 0.5;
    module->envelopes[0].samples[3].amplitude = 0;
    module->envelopes[0].samples[3].duration = 0;
    envelope_init (&module->envelopes[1], 4, LOOP_MODE_NORMAL);
    module->envelopes[1].i_loop_begin = 2;
    module->envelopes[1].i_loop_end = 2;
    module->envelopes[1].samples[0].amplitude = 0;
    module->envelopes[1].samples[0].duration = 0.05;
    module->envelopes[1].samples[1].amplitude = 1;
    module->envelopes[1].samples[1].duration = 0.2;
    module->envelopes[1].samples[2].amplitude = 0.75;
    module->envelopes[1].samples[2].duration = 0.5;
    module->envelopes[1].samples[3].amplitude = 0;
    module->envelopes[1].samples[3].duration = 0;
    envelope_init (&module->envelopes[2], 1, LOOP_MODE_NONE);
    module->envelopes[2].samples[0].amplitude = 1;
    module->envelopes[2].samples[0].duration = 0;
    envelope_init (&module->envelopes[3], 2, LOOP_MODE_NONE);
    module->envelopes[3].samples[0].amplitude = 1;
    module->envelopes[3].samples[0].duration = 0.5;
    module->envelopes[3].samples[1].amplitude = 0;
    module->envelopes[3].samples[1].duration = 0;
    envelope_init (&module->envelopes[4], 5, LOOP_MODE_NORMAL);
    module->envelopes[4].i_loop_begin = 1;
    module->envelopes[4].i_loop_end = 4;
    module->envelopes[4].samples[0].amplitude = 0;
    module->envelopes[4].samples[0].duration = 0.1;
    module->envelopes[4].samples[1].amplitude = 0;
    module->envelopes[4].samples[1].duration = 0.05;
    module->envelopes[4].samples[2].amplitude = 0.1;
    module->envelopes[4].samples[2].duration = 0.1;
    module->envelopes[4].samples[3].amplitude = -0.1;
    module->envelopes[4].samples[3].duration = 0.05;
    module->envelopes[4].samples[4].amplitude = 0;
    module->envelopes[4].samples[4].duration = 0;

    /* define the initial configuration */
    module->configuration.bitmasks[0] = 0xff;
    module->configuration.bitmasks[1] = 0xff;
    module->configuration.flags = 0x00;
    module->configuration.tempo = 90;
    module->configuration.rows_per_beat = 4;
    module->configuration.ticks_per_row = 6;
    module->configuration.volume[0].i_channel = 0;
    module->configuration.volume[0].i_envelope = 0;
    module->configuration.volume[0].multiplier = 0.5;
    module->configuration.volume[4].i_channel = 1;
    module->configuration.volume[4].i_envelope = 1;
    module->configuration.volume[4].multiplier = 0.5;
    module->configuration.frequency[0].i_channel = 0;
    module->configuration.frequency[0].i_envelope = 2;
    module->configuration.frequency[0].multiplier = 0;
    module->configuration.frequency[1].i_channel = 0;
    module->configuration.frequency[1].i_envelope = 2;
    module->configuration.frequency[1].multiplier = 0;
    module->configuration.frequency[4].i_channel = 1;
    module->configuration.frequency[4].i_envelope = 2;
    module->configuration.frequency[4].multiplier = 0;
    module->configuration.frequency[5].i_channel = 1;
    module->configuration.frequency[5].i_envelope = 2;
    module->configuration.frequency[5].multiplier = 1;
    module->configuration.modulation[0].i_channel = 0;
    module->configuration.modulation[0].i_envelope = 3;
    module->configuration.modulation[0].multiplier = 0.5;
    module->configuration.modulation[4].i_channel = 1;
    module->configuration.modulation[4].i_envelope = 3;
    module->configuration.modulation[4].multiplier = 0.5;
}

int main (int argc, char **argv) {

    soundchip_t soundchip;
    module_t module;
    player_t player;
    size_t n_frames;
    double *buffer;
    int result;

    /* initialize the soundchip */
    soundchip_init (&soundchip);

    /* initialize the module */
    module_init (&module,
                 "test module",
                 "somebody",
                 "its a test",
                 2,
                 3,
                 5);

    /* populate the module */
    populate_module (&module);

    /* initialize the module player */
    player_init (&player, &module, &soundchip);

    /* allocate the audio buffer */
    buffer = allocate_audio_buffer (DURATION, AUDIO_RATE, &n_frames);

    /* render the audio */
    render_audio (&player, AUDIO_RATE, n_frames, buffer);

    /* write the audio to a file */
    result = write_audio_to_wav ("test.wav",
                                 buffer,
                                 n_frames,
                                 N_OUTPUTS,
                                 AUDIO_RATE);

    /* clean up the player */
    player_deinit (&player);

    /* clean up the module */
    module_deinit (&module);

    /* clean up the soundchip */
    soundchip_deinit (&soundchip);

    /* free the audio buffer */
    free (buffer);

    return result;
}
