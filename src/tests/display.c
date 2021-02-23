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

int update_size (display_t *display,
                  SDL_Window *window,
                  SDL_Renderer *renderer,
                  SDL_Texture *texture) {

    int width, height;
    SDL_GetWindowSize (window, &width, &height);

    if (texture)
        SDL_DestroyTexture (texture);

    if (!(texture = SDL_CreateTexture (renderer,
                                       SDL_GetWindowPixelFormat (window),
                                       SDL_TEXTUREACCESS_TARGET,
                                       width,
                                       height))) {

        fprintf (stderr, "Could not create texture: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    if (display->buffer_output)
        display_deinit (display);

    display_init (display, width, height);

    return EXIT_FAILURE;
}

int main (int argc, char **argv) {

    /* SDL stuff */
    SDL_Window *window     = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture   = NULL;
    SDL_Event event;

    /* the simulated display */
    display_t display;
    memset (&display, 0, sizeof (display_t));

    /* initialize SDL */
    if (SDL_Init (SDL_INIT_VIDEO) < 0) {

        fprintf (stderr, "Could not initialize SDL: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the window */
    if (!(window = SDL_CreateWindow ("Hashbrown Display",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     WIDTH,
                                     HEIGHT,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))) {

        fprintf (stderr, "Could not create SDL window: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the renderer */
    if (!(renderer = SDL_CreateRenderer (window,
                                         -1,
                                         SDL_RENDERER_ACCELERATED |
                                         SDL_RENDERER_PRESENTVSYNC))) {

        fprintf (stderr, "Could not create SDL renderer: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the render texture and init the display */
    update_size (&display, window, renderer, texture);

    /* run loop */
    while (true) {

        /* handle events */
        while (SDL_PollEvent (&event) != 0)
            switch (event.type) {

                case SDL_WINDOWEVENT:

                    switch (event.window.event) {

                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            update_size (&display, window, renderer, texture);
                            break;
                    }

                    break;

                case SDL_QUIT:
                    goto quit;
            }

        /* render the next image */
        display_process (&display, SDL_GetTicks () / 1000.0);

        /* copy the rendered image to texture memory */
        SDL_UpdateTexture (texture,
                           NULL,
                           display.buffer_output,
                           display.width * sizeof (uint32_t));

        /* present the image on screen */
        SDL_RenderCopy (renderer, texture, NULL, NULL);
        SDL_RenderPresent (renderer);
    }
quit:

    /* free the display */
    display_deinit (&display);

    /* cleanup SDL stuff */
    SDL_DestroyTexture (texture);
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    SDL_Quit ();

    /* exit */
    return EXIT_SUCCESS;
}
