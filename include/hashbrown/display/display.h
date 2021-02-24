#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#define ELECTRONS_PER_SECOND (2400000*50*4*4)
//#define ELECTRONS_PER_SECOND (2400000*50)
#define INTENSITY_PER_SECOND (50000.0*40)
//#define INTENSITY_PER_SECOND (50000.0*10)
#define ELECTRON_SCATTERING 0.5
#define MAX_ELECTRONS_PER_FRAME 400000 /* for real time only */

#define INTENSITY_PER_ELECTRON (INTENSITY_PER_SECOND / ELECTRONS_PER_SECOND)
#define SECONDS_PER_ELECTRON   (1.0 / ELECTRONS_PER_SECOND)

#define PHOSPHOR_TIME_CONSTANT     0.05
#define POWER_SUPPLY_TIME_CONSTANT 0.25

#define PHOSPHOR_REFLECTANCE_RED   0.05
#define PHOSPHOR_REFLECTANCE_GREEN 0.05
#define PHOSPHOR_REFLECTANCE_BLUE  0.05

/* amber */
#define PHOSPHOR_EMITTANCE_RED   1
#define PHOSPHOR_EMITTANCE_GREEN 0.749
#define PHOSPHOR_EMITTANCE_BLUE  0.2

/* green */
//#define PHOSPHOR_EMITTANCE_RED   0.2
//#define PHOSPHOR_EMITTANCE_GREEN 1
//#define PHOSPHOR_EMITTANCE_BLUE  0.2

/* blue */
//#define PHOSPHOR_EMITTANCE_RED   0.2
//#define PHOSPHOR_EMITTANCE_GREEN 0.4
//#define PHOSPHOR_EMITTANCE_BLUE  1

/* red */
//#define PHOSPHOR_EMITTANCE_RED   1
//#define PHOSPHOR_EMITTANCE_GREEN 0.2
//#define PHOSPHOR_EMITTANCE_BLUE  0.2

#define BLOOM_DIAMETER   10
#define BLOOM_SPREAD     100
#define BLOOM_BRIGHTNESS 15
#define BLOOM_RADIUS     (BLOOM_DIAMETER / 2.0)
#define BLOOM_SIZE       (BLOOM_DIAMETER * BLOOM_DIAMETER)

struct display_t;

typedef void display_callback_t (struct display_t *display,
                                 unsigned long int i_electron,
                                 unsigned long int delta_electrons,
                                 void *data);

typedef struct display_t {

    /* electron counter
     * this is the number of electrons that have been emitted
     * since the beginning of time */
    unsigned long int i_electron;

    /* position that the electron gun is aiming at */
    double x, y;

    /* whether the electron gun is shooting beams */
    bool gun_enabled;

    /* power supply */
    double power_supply_in;
    double power_supply_out;

    /* size of the screen in pixels */
    unsigned int width, height;

    /* the bloom kernel */
    double *buffer_kernel;

    /* time of phosphor calculation */
    double *buffer_time;

    /* the buffer of phosphor */
    double *buffer_phosphor;

    /* the output image in RGBA8888 format */
    uint32_t *buffer_output;

    /* the render callback function */
    display_callback_t *callback;

    /* user data to be passed to the callback function */
    void *data;

} display_t;

void display_init    (display_t *display,
                      unsigned int width,
                      unsigned int height,
                      display_callback_t *callback,
                      void *data);
void display_deinit  (display_t *display);
void display_process (display_t *display, double time);

#endif /* DISPLAY_H */
