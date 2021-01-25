/* synth test
 *
 * this is meant to be a low level test of the synth!!!
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <sndfile.h>

#include <hashbrown/sound/synth.h>

#define AUDIO_RATE 44100
#define DURATION 8

double *allocate_audio_buffer (double duration,
                               double audio_rate,
                               size_t *n_frames) {

    *n_frames = duration * audio_rate;
    return calloc (*n_frames, SIZE_FRAME);
}

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
    synth->operators[1].frequency = frequency_1;
    synth->operators[0].modulators[0].modulation_index = 0.75;

    /* modulator 2 */
    synth->operators[4].frequency = frequency_1 * 7;
    synth->operators[0].modulators[1].modulation_index = envelope_2 * 8;

    /* carrier */
    synth->operators[0].frequency = frequency_1;
    synth->operators[0].amplitudes[0] = 0.5 * cos (M_PI * 0.25 / 2);
    synth->operators[0].amplitudes[1] = 0.5 * sin (M_PI * 0.25 / 2);

    /* modulator 1 */
    synth->operators[9].frequency = frequency_2;
    synth->operators[8].modulators[0].modulation_index = 0.75;

    /* carrier */
    synth->operators[8].frequency = frequency_2;
    synth->operators[8].amplitudes[0] = 0.5 * cos (M_PI + M_PI * 0.75 / 2);
    synth->operators[8].amplitudes[1] = 0.5 * sin (M_PI + M_PI * 0.75 / 2);

    /* ring modulator */
    synth->operators[10].frequency = frequency_2 * ring_multiplier;
    synth->operators[8].ring_modulators[0].modulation_index
        = 0.5 * sin (M_PI * time / 2) + 0.5;
    synth->operators[2].frequency = frequency_1 * ring_multiplier;
    synth->operators[0].ring_modulators[0].modulation_index
        = 0.5 * sin (M_PI * time / 2) + 0.5;
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

    SNDFILE *file;
    SF_INFO info;
    int error;

    /* setup the intended audio export format */
    info.samplerate = AUDIO_RATE;
    info.channels = N_OUTPUTS;
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    /* initialize the synth */
    synth_init (&synth);

    /* allocate the audio buffer */
    buffer = allocate_audio_buffer (DURATION, AUDIO_RATE, &n_frames);

    /* render the audio */
    render_audio (&synth, AUDIO_RATE, n_frames, buffer);

    /* open the .wav file */
    if ((file = sf_open ("test.wav", SFM_WRITE, &info)) == NULL) {

        fprintf (stderr,
                 "Unable to open audio file: %s\n",
                 sf_strerror (file));
        return EXIT_FAILURE;
    }

    /* write the sound data to file */
    sf_writef_double (file, buffer, n_frames);

    /* close the .wav file */
    if ((error = sf_close (file))) {

        fprintf (stderr,
                 "Unable to close audio file: %s\n",
                 sf_error_number (error));
        return EXIT_FAILURE;
    }

    /* free the audio buffer */
    free (buffer);

    return EXIT_SUCCESS;
}
