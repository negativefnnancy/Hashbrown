#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <hashbrown/music/module.h>

#include "theme.h"
#include "region.h"
#include "ui/element.h"
#include "ui/dummy.h"
#include "ui/container.h"
#include "ui/text.h"
#include "ui/layout_split.h"

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

    /* all the ui elements */
    struct {

        ui_container_t *pane_1;
        ui_container_t *pane_2;
        ui_container_t *pane_3;
        ui_container_t *pane_4;

        ui_layout_t *layout_1;
        ui_layout_t *layout_2;
        ui_layout_t *layout_3;
        ui_layout_t *layout_4;

        ui_element_t *children_1[2];
        ui_element_t *children_2[2];
        ui_element_t *children_3[3];
        ui_element_t *children_4[3];

        double lengths_1[2];
        double lengths_2[2];
        double lengths_3[3];
        double lengths_4[3];

        ui_dummy_t *view_configuration;
        ui_dummy_t *view_pattern;
        ui_dummy_t *view_metadata;
        ui_dummy_t *view_envelope;

        ui_text_t *button_save;
        ui_text_t *button_load;
        ui_text_t *button_render;

    } ui;

} interface_t;

int interface_init   (interface_t *interface, module_t *module);
int interface_deinit (interface_t *interface);
int interface_run    (interface_t *interface);
int interface_launch (module_t *module);

void interface_repaint (interface_t *interface, region_t region);

#endif /* INTERFACE_H */
