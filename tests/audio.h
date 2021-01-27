#ifndef AUDIO_H
#define AUDIO_H

double *allocate_audio_buffer (double duration,
                               double audio_rate,
                               size_t *n_frames);

int write_audio_to_wav (char *filename,
                        double *buffer,
                        size_t n_frames,
                        unsigned int n_channels,
                        unsigned int audio_rate);

#endif /* AUDIO_H */
