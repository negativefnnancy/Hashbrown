#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sndfile.h>

#include <hashbrown/sound/synth.h>

#define AUDIO_RATE 44100
#define DURATION 1

double *allocate_audio_buffer (double duration,
                               double audio_rate,
                               size_t *n_frames) {

    *n_frames = duration * audio_rate;
    return calloc (*n_frames, SIZE_FRAME);
}

void render_audio (synth_t *synth,
                   double audio_rate,
                   size_t n_frames,
                   double *buffer) {

    size_t i;
    for (i = 0; i < n_frames; i++) {

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
