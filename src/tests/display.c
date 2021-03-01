/* display test
 *
 * this is meant to be a low level test of the vector display!!!
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

#include <hashbrown/display/display.h>

#define WIDTH 480
#define HEIGHT 360

#define MOUSE_FORCE 1
#define MOUSE_DAMPING 1

typedef struct app_t {

    /* SDL stuff */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    /* the simulated display */
    display_t display;

    /* time of init */
    double time_init;

    /* cursor position */
    double mx, my, x, y, vx, vy;
    unsigned long int frame_n_electrons;

    /* oscillators */
    double phase_x, phase_y;

} app_t;

void callback (display_t *display,
               unsigned long int i_electron,
               unsigned long int delta_electrons,
               void *data) {

    app_t *app = (app_t *) data;

    /* mouse pen */
    /*
    double fx = app->mx - app->x;
    double fy = app->my - app->y;
    double ax = fx      * MOUSE_FORCE - app->vx * MOUSE_DAMPING;
    double ay = fy      * MOUSE_FORCE - app->vy * MOUSE_DAMPING;
    app->vx  += ax      / delta_electrons;
    app->vy  += ay      / delta_electrons;
    app->x   += app->vx / delta_electrons;
    app->y   += app->vy / delta_electrons;

    app->display.x = app->x;
    app->display.y = app->y;

    app->display.gun_enabled
        = SDL_GetMouseState (NULL, NULL) & SDL_BUTTON (SDL_BUTTON_LEFT);
        */

    /* lissajous figures */
    const double octave_range = 1;
    //const double base_frequency = 1000;
    //double frequency_x = base_frequency
    //                   * pow (2, app->mx * octave_range / app->display.width  - octave_range / 2);
    //double frequency_y = base_frequency
    //                   * pow (2, app->my * octave_range / app->display.height - octave_range / 2);
    double frequency_x = 250.1 + 125;
    double frequency_y = 1000;
    app->display.x = (sin (app->phase_x * M_PI * 2) * app->display.width  + app->display.width)  / 2;
    app->display.y = (sin (app->phase_y * M_PI * 2) * app->display.height + app->display.height) / 2;
    app->phase_x += frequency_x * SECONDS_PER_ELECTRON;
    app->phase_y += frequency_y * SECONDS_PER_ELECTRON;
    app->display.intensity = 1;
}

int update_size (app_t *app) {

    int width, height;
    SDL_GetWindowSize (app->window, &width, &height);

    if (app->texture)
        SDL_DestroyTexture (app->texture);

    if (!(app->texture = SDL_CreateTexture (app->renderer,
                                            SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            width,
                                            height))) {

        fprintf (stderr, "Could not create texture: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    if (app->display.buffer_output)
        display_deinit (&app->display);

    display_init (&app->display, width, height, callback, app);

    app->time_init = SDL_GetTicks () / 1000.0;

    return EXIT_FAILURE;
}

int main (int argc, char **argv) {

    app_t app;
    SDL_Event event;

    memset (&app, 0, sizeof (app));

    /* initialize SDL */
    if (SDL_Init (SDL_INIT_VIDEO) < 0) {

        fprintf (stderr, "Could not initialize SDL: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the window */
    if (!(app.window = SDL_CreateWindow ("Hashbrown Display",
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         WIDTH,
                                         HEIGHT,
                                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))) {

        fprintf (stderr, "Could not create SDL window: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the renderer */
    if (!(app.renderer = SDL_CreateRenderer (app.window,
                                             -1,
                                             SDL_RENDERER_ACCELERATED |
                                             SDL_RENDERER_PRESENTVSYNC))) {

        fprintf (stderr, "Could not create SDL renderer: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the render texture and init the display */
    update_size (&app);

    /* run loop */
    while (true) {

        /* handle events */
        while (SDL_PollEvent (&event) != 0)
            switch (event.type) {

                case SDL_WINDOWEVENT:

                    switch (event.window.event) {

                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            update_size (&app);
                            break;
                    }

                    break;

                case SDL_QUIT:
                    goto quit;

                case SDL_KEYDOWN:

                    switch (event.key.keysym.sym) {

                        case SDLK_ESCAPE:
                            goto quit;

                        case SDLK_TAB:
                            app.display.power_supply_in = !app.display.power_supply_in;
                    }

                    break;

                case SDL_MOUSEMOTION:
                    app.mx = event.motion.x;
                    app.my = event.motion.y;
                    break;
            }

        /* render the next image */
        display_process (&app.display, SDL_GetTicks () / 1000.0 - app.time_init);

        /* copy the rendered image to texture memory */
        SDL_UpdateTexture (app.texture,
                           NULL,
                           app.display.buffer_output,
                           app.display.width * sizeof (uint32_t));

        /* present the image on screen */
        SDL_SetRenderTarget (app.renderer, NULL);
        SDL_RenderCopy (app.renderer, app.texture, NULL, NULL);
        SDL_RenderPresent (app.renderer);
    }
quit:

    /* free the display */
    display_deinit (&app.display);

    /* cleanup SDL stuff */
    SDL_DestroyTexture (app.texture);
    SDL_DestroyRenderer (app.renderer);
    SDL_DestroyWindow (app.window);
    SDL_Quit ();

    /* exit */
    return EXIT_SUCCESS;
}
