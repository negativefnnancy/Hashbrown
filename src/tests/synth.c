/* synth test
 *
 * this is meant to be a low level test of the synth!!!
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hashbrown/sound/synth.h>

#include "audio.h"

#define AUDIO_RATE 44100
#define DURATION 8

void update_synth (synth_t *synth, double time) {

    double envelope_1, envelope_2;
    double frequency_1, frequency_2, frequency_3;
    double note_time;
    double ring_multiplier;
    int i_note;

    note_time = time;
    while (note_time > 0.5)
        note_time -= 0.5;

    i_note = (int) (time * 2) % 8;

    ring_multiplier = time > 4 ? 1.5 : 2;

    envelope_1 = 1 - time / DURATION;

    envelope_2 = 1 - note_time;
    if (envelope_2 < 0)
        envelope_2 = 0;

    frequency_1 = 61.41 / 2 * (i_note + 2);
    frequency_2 = 61.41 / 2 * (i_note + 4);
    frequency_3 = frequency_2 / 2;

    /* modulator 1 */
/*    synth->operators[1].frequency = frequency_1;
    synth->operators[0].modulators[0].modulation_index = 0.75; */

    /* modulator 2 */
/*    synth->operators[2].frequency = frequency_1 * 7;
    synth->operators[0].modulators[1].modulation_index = envelope_2 * 8;*/

    /* carrier */
/*    synth->operators[0].frequency = frequency_1;
    synth->operators[0].amplitudes[0] = 0.0 * cos (M_PI * 0.25 / 2);
    synth->operators[0].amplitudes[1] = 0.0 * sin (M_PI * 0.25 / 2);*/

    /* modulator 1 */
/*    synth->operators[5].frequency = frequency_2;
    synth->operators[4].modulators[0].modulation_index = 0.75;*/

    /* carrier */
/*    synth->operators[4].frequency = frequency_2;
    synth->operators[4].amplitudes[0] = 0.0 * cos (M_PI + M_PI * 0.75 / 2);
    synth->operators[4].amplitudes[1] = 0.0 * sin (M_PI + M_PI * 0.75 / 2); */

    /* ring modulator */
/*    synth->operators[3].frequency = frequency_2 * ring_multiplier;
    synth->operators[4].ring_modulators[0].modulation_index
        = 0.5 * sin (M_PI * time / 2) + 0.5;
    synth->operators[11].frequency = frequency_1 * ring_multiplier;
    synth->operators[0].ring_modulators[0].modulation_index
        = 0.5 * sin (M_PI * time / 2) + 0.5; */

    /* noise */
    synth->operators[10].frequency = 200;
    synth->operators[10].noise_mode = true;
    synth->operators[10].amplitudes[0] = 0.0;
    synth->operators[10].amplitudes[1] = 0.0;

    /* noise modulated tone */
    synth->operators[9].frequency = 220;
    synth->operators[8].modulators[0].modulation_index = envelope_1 * 3.14;
    synth->operators[8].frequency = 440;
    synth->operators[8].amplitudes[0] = 0.5;
    synth->operators[8].amplitudes[1] = 0.5;
}

void render_audio (synth_t *synth,
                   double audio_rate,
                   size_t n_frames,
                   double *buffer) {

    size_t i;
    for (i = 0; i < n_frames; i++) {

        update_synth (synth, i / audio_rate);
        synth_process (synth, audio_rate);
        memcpy (buffer, synth->outputs, SIZE_FRAME);
        buffer += N_OUTPUTS;
    }
}

int main (int argc, char **argv) {

    synth_t synth;
    size_t n_frames;
    double *buffer;
    int result;

    /* initialize the synth */
    synth_init (&synth);

    /* allocate the audio buffer */
    buffer = allocate_audio_buffer (DURATION, AUDIO_RATE, &n_frames);

    /* render the audio */
    render_audio (&synth, AUDIO_RATE, n_frames, buffer);

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
