/* display test
 *
 * this is meant to be a low level test of the vector display!!!
 */

#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include <hashbrown/display/display.h>

#define WIDTH 480
#define HEIGHT 360

int main (int argc, char **argv) {

    /* SDL stuff */
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Event event;

    /* the simulated display */
    display_t display;

    /* initialize SDL */
    if (SDL_Init (SDL_INIT_VIDEO) < 0) {

        fprintf (stderr, "Could not initialize SDL: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the window */
    if (!(window == SDL_CreateWindow ("Hashbrown Display",
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      WIDTH,
                                      HEIGHT,
                                      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))) {

        fprintf (stderr, "Could not create SDL window: %s\n", SDL_Error ());
        return EXIT_FAILURE;
    }

    /* initialize the display */
    display_init (&display);

    /* run loop */
    while (true) {

        /* handle events */
        while (SDL_PollEvent (&event) != 0)
            switch (event.type) {

                case SDL_QUIT:
                    goto quit;
    }
quit:

    /* free the display */
    display_deinit (&display);

    /* cleanup SDL stuff */
    SDL_DestroyWindow (window);
    SDL_Quit ();

    /* exit */
    return EXIT_SUCCESS;
}
