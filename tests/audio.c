#include <stdlib.h>

#include <sndfile.h>

#include <hashbrown/sound/config.h>

#include "audio.h"

double *allocate_audio_buffer (double duration,
                               double audio_rate,
                               size_t *n_frames) {

    *n_frames = duration * audio_rate;
    return calloc (*n_frames, SIZE_FRAME);
}

int write_audio_to_wav (char *filename,
                         double *buffer,
                         size_t n_frames,
                         unsigned int n_channels,
                         unsigned int audio_rate) {

    SNDFILE *file;
    SF_INFO info;
    int error;

    /* setup the intended audio export format */
    info.samplerate = audio_rate;
    info.channels = n_channels;
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    /* open the .wav file */
    if ((file = sf_open (filename, SFM_WRITE, &info)) == NULL) {

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

    return EXIT_SUCCESS;
}
