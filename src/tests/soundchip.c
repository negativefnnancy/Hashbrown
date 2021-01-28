/* soundchip test
 *
 * this is meant to be a low level test of the soundchip!!!
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hashbrown/sound/soundchip.h>

#include "audio.h"

#define AUDIO_RATE 48000
#define DURATION 4

void update_soundchip (soundchip_t *soundchip, double time) {

    double in_sec = time;
    while (in_sec >= 1)
        in_sec -= 1;
    double ramp = 1 - in_sec * 2;
    if (ramp < 0.1)
        ramp = 0.1;

    /* update the registers */
    double v = 220 * pow (2, sin (M_PI * 2 * 6 * time) * 0.04);
    double v2 = 440 * pow (2, sin (M_PI * 2 * 1 * time) * 0.04);
    /*soundchip_set_frequency  (soundchip, 0, v);
    soundchip_set_frequency  (soundchip, 1, v / 2);
    soundchip_set_frequency  (soundchip, 2, v * 2);
    soundchip_set_frequency  (soundchip, 4, v * 3);
    soundchip_set_frequency  (soundchip, 5, v / 2 * 2.5);
    soundchip_set_frequency  (soundchip, 6, v * 2 * 2.5);
    soundchip_set_modulation (soundchip, 0, ramp / 2);
    soundchip_set_modulation (soundchip, 12, time / DURATION / 8);
    soundchip_set_modulation (soundchip, 4, ramp / 4);
    soundchip_set_modulation (soundchip, 16, time / DURATION / 4);
    soundchip_set_volume     (soundchip, 0, 0.5, 0.5);
    soundchip_set_volume     (soundchip, 4, 0.5, 0.5);*/
    soundchip_set_frequency (soundchip, 0, 220 * pow (2, 10 * time / DURATION));
    soundchip_set_frequency (soundchip, 1, 220 * pow (2, 10 * time / DURATION));
    soundchip_set_modulation (soundchip, 0, 1);
    soundchip_set_volume (soundchip, 0, 0.5, 0.5);
    soundchip->flags_register = 0;

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
