#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <hashbrown/music/module.h>

#include "ui/element.h"
#include "theme.h"
#include "region.h"

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480

typedef struct interface_t {

    module_t *module;         /* the tracker module being edited */
    SDL_Window *window;       /* the window showing this interface */
    SDL_Renderer *renderer;   /* the renderer used for drawing */
    SDL_Texture *framebuffer; /* the texture to render to */
    TTF_Font *font;           /* the font used for text rendering */
    theme_t theme;            /* the theme to render the interface with */
    region_t window_region;   /* the region of the entire window */
    region_t repaint_region;  /* the region of the window to redraw on vsync */
    ui_element_t *root;       /* the ui element at the root of the window */
    bool running;             /* whether the interface is still running */

} interface_t;

int interface_init   (interface_t *interface, module_t *module);
int interface_deinit (interface_t *interface);
int interface_run    (interface_t *interface);
int interface_launch (module_t *module);

void interface_repaint (interface_t *interface, region_t region);

#endif /* INTERFACE_H */
