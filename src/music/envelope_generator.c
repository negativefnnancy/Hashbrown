#include <stdlib.h>
#include <string.h>

#include <hashbrown/music/envelope_generator.h>

void envelope_generator_init (envelope_generator_t *envelope_generator) {

    memset (envelope_generator, 0, sizeof (envelope_generator_t));
}

double interpolate (double a, double b, double mix) {

    return a + mix * (b - a);
}

double get_sample_time (envelope_t *envelope, size_t i_sample) {

    double time;
    size_t i;
    for (i = 0, time = 0; i < i_sample; time += envelope->samples[i++].duration);
    return time;
}

double double_modulo (double a, double b) {

    while (a >= b)
        a -= b;
    return a;
}

void envelope_generator_process (envelope_generator_t *envelope_generator,
                                 envelope_t *envelope,
                                 double delta_time) {

    double mix;
    double time_a, time_b;
    size_t i_sample_a, i_sample_b;
    envelope_sample_t sample_a, sample_b;

    do {

        /* determine the indices of which samples we are between */
        i_sample_a = 0;
        i_sample_b = 0;
        time_a = 0;
        time_b = 0;
        while (time_b <= envelope_generator->phase) {

            time_a = time_b;
            time_b += envelope->samples[i_sample_b++].duration;
            if (i_sample_b == envelope->n_samples)
                break;
        }

        i_sample_a = i_sample_b - 1;
        if (i_sample_b == envelope->n_samples) {
            i_sample_b = i_sample_a;
            time_b = time_a;
        }

        if (envelope->loop_mode == LOOP_MODE_NORMAL
            && i_sample_a >= envelope->i_loop_end) {

            double loop_begin_time
                = get_sample_time (envelope, envelope->i_loop_begin);
            double loop_end_time
                = get_sample_time (envelope, envelope->i_loop_end);
            double loop_duration = loop_end_time - loop_begin_time;
            if (loop_duration)
                envelope_generator->phase
                    = loop_begin_time
                    + double_modulo (envelope_generator->phase - loop_begin_time,
                                     loop_duration);
            else
                envelope_generator->phase = loop_begin_time;

            continue;
        }

    } while (false);

    /* get the samples */
    sample_a = envelope->samples[i_sample_a];
    sample_b = envelope->samples[i_sample_b];

    /* linear interpolate between the two sample values */
    if (time_b == time_a) {
        envelope_generator->output = sample_a.amplitude;
    } else {
        mix = (envelope_generator->phase - time_a) / (time_b - time_a);
        envelope_generator->output = interpolate (sample_a.amplitude,
                                                  sample_b.amplitude,
                                                  mix);
    }

    envelope_generator->phase += delta_time;
}

void envelope_generator_note_on  (envelope_generator_t *envelope_generator) {

    envelope_generator->phase = 0;
    envelope_generator->sustain = true;
}

void envelope_generator_note_off (envelope_generator_t *envelope_generator) {

    envelope_generator->sustain = false;
}
