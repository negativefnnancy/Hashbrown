/* soundchip test
 *
 * this is meant to be a low level test of the soundchip!!!
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hashbrown/sound/soundchip.h>

#include "audio.h"

#define AUDIO_RATE 44100
#define DURATION 4

void update_soundchip (soundchip_t *soundchip, double time) {

    double in_sec = time;
    while (in_sec >= 1)
        in_sec -= 1;
    double ramp = 1 - in_sec * 1;
    if (ramp < 0)
        ramp = 0;

    /* update the registers */
    soundchip_set_frequency  (soundchip, 0, 440);
    soundchip_set_frequency  (soundchip, 1, 220);
    soundchip_set_frequency  (soundchip, 2, 440);
    soundchip_set_modulation (soundchip, 0, ramp);
    /*soundchip_set_modulation (soundchip, 12, time / DURATION);*/
    soundchip_set_volume     (soundchip, 0, 0.25, 0.25);

    /* tell the soundchip to accept the register values */
    soundchip_flush_registers (soundchip);
}

void render_audio (soundchip_t *soundchip,
                   double audio_rate,
                   size_t n_frames,
                   double *buffer) {

    size_t i;
    for (i = 0; i < n_frames; i++) {

        update_soundchip (soundchip, i / audio_rate);
        soundchip_process (soundchip, audio_rate);
        memcpy (buffer, soundchip->outputs, SIZE_FRAME);
        buffer += N_OUTPUTS;
    }
}

int main (int argc, char **argv) {

    soundchip_t soundchip;
    size_t n_frames;
    double *buffer;
    int result;

    /* initialize the soundchip */
    soundchip_init (&soundchip);

    /* allocate the audio buffer */
    buffer = allocate_audio_buffer (DURATION, AUDIO_RATE, &n_frames);

    /* render the audio */
    render_audio (&soundchip, AUDIO_RATE, n_frames, buffer);

    /* write the audio to a file */
    result = write_audio_to_wav ("test.wav",
                                 buffer,
                                 n_frames,
                                 N_OUTPUTS,
                                 AUDIO_RATE);

    /* free the audio buffer */
    free (buffer);

    return result;
}
