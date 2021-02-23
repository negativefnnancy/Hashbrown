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

/* TODO consolodate this demo with the display.c demo */

typedef struct app_t {

    /* SDL stuff */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    /* the simulated display */
    display_t display;

    /* time of init */
    double time_init;

} app_t;

extern double noise ();

void callback (display_t *display,
               unsigned long int i_electron,
               unsigned long int delta_electrons,
               void *data) {

    app_t *app = (app_t *) data;

    app->display.gun_enabled = true;

    double angle = noise () * M_PI * 2;
    double radius = app->display.width * sqrt (noise ()) / 4;
    app->display.x = cos (angle) * radius + app->display.width / 2;
    app->display.y = sin (angle) * radius + app->display.height / 2;
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
    if (!(app.window = SDL_CreateWindow ("Hashbrown Raster Test",
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
