#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hashbrown/display/display.h>

void generate_kernel (display_t *display) {

    size_t i;
    for (i = 0; i < BLOOM_SIZE; i++) {

        double x        = i % BLOOM_DIAMETER;
        double y        = i / BLOOM_DIAMETER;
        double offset_x = x - BLOOM_RADIUS;
        double offset_y = y - BLOOM_RADIUS;
        double radius   = sqrt (offset_x * offset_x + offset_y * offset_y) / BLOOM_DIAMETER;
        double value    = pow (radius, 1.0 / BLOOM_SPREAD);
        display->buffer_kernel[i] = fmax (0, 1 - value);
    }
}

void display_init (display_t *display,
                   unsigned int width,
                   unsigned int height,
                   display_callback_t *callback,
                   void *data) {

    memset (display, 0, sizeof (display_t));
    display->width  = width;
    display->height = height;
    display->buffer_kernel   = calloc (BLOOM_SIZE,     sizeof (double));
    display->buffer_phosphor = calloc (width * height, sizeof (double));
    display->buffer_time     = calloc (width * height, sizeof (double));
    display->buffer_output   = calloc (width * height, sizeof (uint32_t));
    display->callback = callback;
    display->data = data;
    display->power_supply_in = 1;
    generate_kernel (display);
}

void display_deinit (display_t *display) {

    free (display->buffer_kernel);
    free (display->buffer_phosphor);
    free (display->buffer_time);
    free (display->buffer_output);
}

double noise () {

    return (double) rand () / RAND_MAX;
}

double linear_interpolate (double a, double b, double amount) {

    return a + (b - a) * amount;
}

double get_phosphor_pixel (display_t *display, size_t i, double time) {

    double delta_time = time - display->buffer_time[i];
    return display->buffer_phosphor[i] * exp (-delta_time / PHOSPHOR_TIME_CONSTANT);
}

void display_emit_electron (display_t *display, double time) {

    /* update power supply */
    display->power_supply_out
        = display->power_supply_in - (display->power_supply_in - display->power_supply_out)
        * exp (-SECONDS_PER_ELECTRON / POWER_SUPPLY_TIME_CONSTANT);

    /* electron scattering */
    double scatter_radius = tan (noise () * 2) * ELECTRON_SCATTERING;
    double scatter_angle  = noise () * M_PI * 2;
    double scatter_x      = cos (scatter_angle) * scatter_radius;
    double scatter_y      = sin (scatter_angle) * scatter_radius;

    /* electron position */
    double x = linear_interpolate (display->width  / 2, 
                                   display->x + scatter_x,
                                   display->power_supply_out);
    double y = linear_interpolate (display->height / 2,
                                   display->y + scatter_y,
                                   display->power_supply_out);

    /* ignore electrons that miss the screen */
    if (x < 0 || y < 0 || x >= display->width || y >= display->height)
        return;

    /* phosphor */
    {
        double intensity = INTENSITY_PER_ELECTRON * display->power_supply_out;
        size_t i_phosphor = (int) x + (int) y * display->width;
        display->buffer_phosphor[i_phosphor]
            = get_phosphor_pixel (display, i_phosphor, time) + intensity;
        display->buffer_time[i_phosphor] = time;
    }
}

uint8_t color_channel_clamp (double intensity) {

    int scaled = intensity * 255;
    if (scaled < 0)
        return 0;
    if (scaled > 255)
        return 255;
    return scaled;
}

uint32_t color_format (double red, double green, double blue, double alpha) {

    uint8_t red_clamp   = color_channel_clamp (red);
    uint8_t green_clamp = color_channel_clamp (green);
    uint8_t blue_clamp  = color_channel_clamp (blue);
    uint8_t alpha_clamp = color_channel_clamp (alpha);
    return (red_clamp   << 24) |
           (green_clamp << 16) |
           (blue_clamp  << 8)  |
           (alpha_clamp << 0);
}

uint32_t color_map (double intensity) {

    double red   = intensity * PHOSPHOR_EMITTANCE_RED   + PHOSPHOR_REFLECTANCE_RED;
    double green = intensity * PHOSPHOR_EMITTANCE_GREEN + PHOSPHOR_REFLECTANCE_GREEN;
    double blue  = intensity * PHOSPHOR_EMITTANCE_BLUE  + PHOSPHOR_REFLECTANCE_BLUE;
    return color_format (red, green, blue, 1);
}

void display_render (display_t *display, double time) {

    /* TODO rn this is just copying PFFFFFFFTTTTTTTTTTTTTTTTTTTTTTTT LOL */
    size_t i, j;
    size_t n_pixels = display->width * display->height;
    for (i = 0; i < n_pixels; i++) {

        /* accumulate pixel brightness */
        double intensity = 0;

        if (BLOOM_DIAMETER > 0) {

            /* pixel position */
            double pixel_x = i % display->width;
            double pixel_y = i / display->width;

            /* convolve */
            for (j = 0; j < BLOOM_SIZE; j++) {

                /* get the kernel position */
                double x = j % BLOOM_DIAMETER;
                double y = j / BLOOM_DIAMETER;

                /* get the corresponding source position */
                double offset_x = x - BLOOM_RADIUS;
                double offset_y = y - BLOOM_RADIUS;
                double source_x = pixel_x + offset_x;
                double source_y = pixel_y + offset_y;

                /* ignore out of bounds pixels */
                if (source_x < 0               ||
                    source_y < 0               ||
                    source_x >= display->width ||
                    source_y >= display->height)
                    continue;

                /* get indices */
                size_t i_kernel = (int) x + (int) y * (int) BLOOM_DIAMETER;
                size_t i_source = (int) source_x + (int) source_y * (int) display->width;

                /* sample kernel and phosphor */
                double sample_kernel = display->buffer_kernel   [i_kernel];
                double sample_source = get_phosphor_pixel (display, i_source, time);

                /* accumulate the convolution */
                intensity += sample_source * sample_kernel * BLOOM_BRIGHTNESS;
            }

        } else
            intensity = get_phosphor_pixel (display, i, time);
        
        /* color the pixel */
        display->buffer_output[i] = color_map (intensity);
    }
}

void display_process (display_t *display, double time) {

    unsigned long int i_electron, delta_electrons;
    size_t i;

    /* calculate how many electrons should be emitted */
    i_electron = time * ELECTRONS_PER_SECOND;
    delta_electrons = i_electron - display->i_electron;

    /* emit the amount of electrons */
    for (i = 0; i < delta_electrons; i++) {

        /* update the display state */
        display->callback (display, i, delta_electrons, display->data);

        /* emit the electron */
        if (display->gun_enabled)
            display_emit_electron (display,
                                   time
                                   - (delta_electrons - i)
                                   / (double) ELECTRONS_PER_SECOND);
    }

    /* apply the bloom filter */
    display_render (display, time);

    /* update electron counter */
    display->i_electron = i_electron;
}
