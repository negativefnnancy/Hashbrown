#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interface.h"

int interface_init (interface_t *interface, module_t *module) {

    memset (interface, 0, sizeof (interface_t));
    interface->module = module;

    /* initialize SDL */
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {

        fprintf (stderr, "Could not initialize SDL: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* initialize fonts */
    if (TTF_Init () < 0) {

        fprintf (stderr, "Could not initialize TTF: %s\n", TTF_GetError ());
        return EXIT_FAILURE;
    }

    /* load the font */
    /* TODO investigate the best way to bundle resource files with distribution */
    if ((interface->font = TTF_OpenFont ("font.ttf", 12)) == NULL) {

        fprintf (stderr, "Could not open font: %s\n", TTF_GetError ());
        return EXIT_FAILURE;
    }

    /* create the window */
    if ((interface->window = SDL_CreateWindow ("Hashbrown Tracker",
                                               SDL_WINDOWPOS_UNDEFINED,
                                               SDL_WINDOWPOS_UNDEFINED,
                                               DEFAULT_SCREEN_WIDTH,
                                               DEFAULT_SCREEN_HEIGHT,
                                               SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))
            == NULL) {

        fprintf (stderr, "Could not create SDL window: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
 
int interface_deinit (interface_t *interface) {

    /* clean up SDL */
    SDL_DestroyWindow (interface->window);
    SDL_Quit ();

    /* clean up fonts */
    TTF_CloseFont (interface->font);
    TTF_Init ();

    return EXIT_SUCCESS;
}

void interface_draw (interface_t *interface) {

    SDL_Surface *surface;

    /* the window surface */
    surface = SDL_GetWindowSurface (interface->window);

    Uint32 black = SDL_MapRGB (surface->format, 32, 32, 32);

    /* TODO: everything */

    /* draw the background */
    SDL_FillRect (surface, NULL, black);

    /* present the image to the screen */
    SDL_UpdateWindowSurface (interface->window);
}

int interface_run (interface_t *interface) {

    SDL_Event event;

    while (SDL_WaitEvent (&event)) {

        SDL_PumpEvents ();

        switch (event.type) {

            case SDL_WINDOWEVENT:

                switch (event.window.event) {

                    case SDL_WINDOWEVENT_SHOWN:
                        interface_draw (interface);
                        break;

                    case SDL_WINDOWEVENT_EXPOSED:
                        /* TODO only redraw damaged regions */
                        interface_draw (interface);
                        break;

                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        interface_draw (interface);
                        break;
                }

                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {

                    case SDLK_ESCAPE:
                        return EXIT_SUCCESS;

                    case SDLK_F5:
                        break;

                    case SDLK_F6:
                        break;

                    case SDLK_F7:
                        break;

                    case SDLK_c:
                        break;

                    case SDLK_v:
                        break;

                    case SDLK_UP:
                        break;

                    case SDLK_DOWN:
                        break;

                    case SDLK_LEFT:
                        break;

                    case SDLK_RIGHT:
                        break;

                    case SDLK_BACKSPACE:
                        break;

                    case SDLK_DELETE:
                        break;

                    case SDLK_0:
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                    case SDLK_5:
                    case SDLK_6:
                    case SDLK_7:
                    case SDLK_8:
                    case SDLK_9:
                    case SDLK_EQUALS:
                        break;

                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        break;

                    case SDLK_CAPSLOCK:
                        break;
                }

                break;

            case SDL_TEXTINPUT:
                break;

            case SDL_MOUSEMOTION:
                break;

            case SDL_MOUSEBUTTONDOWN:
                break;

            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_EXPOSED:
            case SDL_WINDOWEVENT_RESIZED:
                break;

            case SDL_QUIT:
                return EXIT_SUCCESS;

            default:
                break;
        }
    }

    return EXIT_SUCCESS;
}

int interface_launch (module_t *module) {

    int code;
    interface_t interface;

    /* initialize */
    if ((code = interface_init (&interface, module)))
        return code;

    /* main loop */
    if ((code = interface_run (&interface)))
        return code;

    /* clean up */
    if ((code = interface_deinit (&interface)))
        return code;

    return EXIT_SUCCESS;
}
