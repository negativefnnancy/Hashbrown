/* music test
 *
 * this is meant to be a low level test of the music driver!!!!
 */

#include <hashbrown/sound/soundchip.h>
#include <hashbrown/music/module.h>
#include <hashbrown/music/player.h>

#include "audio.h"

#define AUDIO_RATE 48000
#define DURATION 4

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
                 4,
                 8,
                 4);

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

    /* clean up the module */
    module_deinit (&module);

    /* clean up the soundchip */
    soundchip_deinit (&soundchip);

    /* free the audio buffer */
    free (buffer);

    return result;
}
