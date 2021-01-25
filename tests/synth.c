#include <stdlib.h>
#include <stdio.h>

#include <sndfile.h>

#include <hashbrown/sound/synth.h>

#include "util.h"

int main (int argc, char **argv) {

    synth_t synth;

    SNDFILE *file;
    SF_INFO info;
    int error;

    /* open the .wav file */
    if ((file = sf_open ("test.wav", SFM_WRITE, &info)) == NULL) {

        fprintf (stderr,
                 "Unable to open audio file: %s\n",
                 sf_strerror (file));
        return EXIT_FAILURE;
    }

    /* close the .wav file */
    if ((error = sf_close (file))) {

        fprintf (stderr,
                 "Unable to close audio file: %s\n",
                 sf_error_number (error));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
