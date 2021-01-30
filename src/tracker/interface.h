#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <hashbrown/music/module.h>

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480

typedef struct interface_t {

    module_t *module;   /* the tracker module being edited */
    SDL_Window *window; /* the window showing this interface */
    TTF_Font *font;     /* the font used for text rendering */

} interface_t;

int interface_init   (interface_t *interface, module_t *module);
int interface_deinit (interface_t *interface);
int interface_run    (interface_t *interface);
int interface_launch (module_t *module);

#endif /* INTERFACE_H */
