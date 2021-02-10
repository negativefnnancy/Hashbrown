#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interface.h"
#include "ui/event.h"

void interface_construct (interface_t *interface) {

    ui_box_style_t style_1, style_2, style_3;

    if (interface->root) {

        /* TODO */
/*        ui_container_destroy ((ui_container_t *) interface->root);
        ui_dummy_destroy     (interface->dummy_1);
        ui_dummy_destroy     (interface->dummy_2);
        ui_dummy_destroy     (interface->dummy_3);*/
    }

    style_1 = ui_box_style_make (0,
                                 0,
                                 interface->theme.spacing, 
                                 interface->theme.colors[0],
                                 interface->theme.colors[0],
                                 interface->theme.colors[0]);
    style_2 = ui_box_style_make (0,
                                 1,
                                 0, 
                                 interface->theme.colors[0],
                                 interface->theme.colors[2],
                                 interface->theme.colors[0]);
    style_3 = ui_box_style_make (0,
                                 0,
                                 0,
                                 interface->theme.colors[0],
                                 interface->theme.colors[0],
                                 interface->theme.colors[0]);

    interface->ui.view_configuration = ui_dummy_create (interface->theme.colors[0], style_2);
    interface->ui.view_pattern       = ui_dummy_create (interface->theme.colors[0], style_2);
    interface->ui.view_metadata      = ui_dummy_create (interface->theme.colors[0], style_2);
    interface->ui.view_envelope      = ui_dummy_create (interface->theme.colors[0], style_2);

    interface->ui.button_save   = ui_text_create (style_2,
                                                  interface->theme.colors[0],
                                                  interface->theme.colors[2],
                                                  "save",
                                                  TEXT_ALIGNMENT_CENTER);
    interface->ui.button_load   = ui_text_create (style_2,
                                                  interface->theme.colors[0],
                                                  interface->theme.colors[2],
                                                  "load",
                                                  TEXT_ALIGNMENT_CENTER);
    interface->ui.button_render = ui_text_create (style_2,
                                                  interface->theme.colors[0],
                                                  interface->theme.colors[2],
                                                  "render",
                                                  TEXT_ALIGNMENT_CENTER);

    interface->ui.layout_1
        = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_VERTICAL,
                                                  interface->theme.spacing,
                                                  interface->ui.lengths_1);
    interface->ui.layout_2
        = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_HORIZONTAL,
                                                  interface->theme.spacing,
                                                  interface->ui.lengths_2);
    interface->ui.layout_3
        = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_VERTICAL,
                                                  interface->theme.spacing,
                                                  interface->ui.lengths_3);
    interface->ui.layout_4
        = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_HORIZONTAL,
                                                  interface->theme.spacing,
                                                  interface->ui.lengths_4);

    interface->ui.pane_1 = ui_container_create (style_1,
                                                interface->ui.layout_1,
                                                2,
                                                interface->ui.children_1);
    interface->ui.pane_2 = ui_container_create (style_3,
                                                interface->ui.layout_2,
                                                2,
                                                interface->ui.children_2);
    interface->ui.pane_3 = ui_container_create (style_3,
                                                interface->ui.layout_3,
                                                3,
                                                interface->ui.children_3);
    interface->ui.pane_4 = ui_container_create (style_3,
                                                interface->ui.layout_4,
                                                3,
                                                interface->ui.children_4);

    interface->ui.lengths_1[0] = 128;
    interface->ui.lengths_1[1] = 0;

    interface->ui.lengths_2[0] = 0;
    interface->ui.lengths_2[1] = 128;

    interface->ui.lengths_3[0] = 128;
    interface->ui.lengths_3[1] = 0;
    interface->ui.lengths_3[2] = 24;

    interface->ui.lengths_4[0] = 38;
    interface->ui.lengths_4[1] = 38;
    interface->ui.lengths_4[2] = 0;

    interface->ui.children_1[0] = (ui_element_t *) interface->ui.view_configuration;
    interface->ui.children_1[1] = (ui_element_t *) interface->ui.pane_2;

    interface->ui.children_2[0] = (ui_element_t *) interface->ui.view_pattern;
    interface->ui.children_2[1] = (ui_element_t *) interface->ui.pane_3;

    interface->ui.children_3[0] = (ui_element_t *) interface->ui.view_metadata;
    interface->ui.children_3[1] = (ui_element_t *) interface->ui.view_envelope;
    interface->ui.children_3[2] = (ui_element_t *) interface->ui.pane_4;

    interface->ui.children_4[0] = (ui_element_t *) interface->ui.button_save;
    interface->ui.children_4[1] = (ui_element_t *) interface->ui.button_load;
    interface->ui.children_4[2] = (ui_element_t *) interface->ui.button_render;

    interface->root = (ui_element_t *) interface->ui.pane_1;
}

void interface_set_theme (interface_t *interface, theme_t theme) {

    interface->theme = theme;
    interface_construct (interface);
}

int interface_update_window_region (interface_t *interface) {

    int width, height;

    SDL_GetWindowSize (interface->window, &width, &height);

    interface->window_region.position   = vec2_make (0, 0);
    interface->window_region.dimensions = vec2_make (width, height);

    /* recreate the framebuffer texture */
    if (interface->framebuffer)
        SDL_DestroyTexture (interface->framebuffer);

    if ((interface->framebuffer
                = SDL_CreateTexture (interface->renderer,
                                     SDL_GetWindowPixelFormat (interface->window),
                                     SDL_TEXTUREACCESS_TARGET,
                                     width,
                                     height))
            == NULL) {

        fprintf (stderr, "Could not create texture: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int interface_init (interface_t *interface, module_t *module) {

    memset (interface, 0, sizeof (interface_t));
    interface->module = module;
    interface->running = true;

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
                                               SDL_WINDOW_SHOWN |
                                               SDL_WINDOW_RESIZABLE))
            == NULL) {

        fprintf (stderr, "Could not create SDL window: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the renderer */
    if ((interface->renderer = SDL_CreateRenderer (interface->window,
                                                   -1,
                                                   SDL_RENDERER_ACCELERATED |
                                                   SDL_RENDERER_PRESENTVSYNC))
            == NULL) {

        fprintf (stderr, "Could not create SDL renderer: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    interface_update_window_region (interface);
    interface_set_theme (interface, default_theme);

    return EXIT_SUCCESS;
}
 
int interface_deinit (interface_t *interface) {

    /* clean up SDL */
    SDL_DestroyTexture  (interface->framebuffer);
    SDL_DestroyRenderer (interface->renderer);
    SDL_DestroyWindow   (interface->window);
    SDL_Quit ();

    /* clean up fonts */
    TTF_CloseFont (interface->font);
    TTF_Init ();

    return EXIT_SUCCESS;
}

void interface_render (interface_t *interface) {

    /* set the render target to the framebuffer texture */
    SDL_SetRenderTarget (interface->renderer, interface->framebuffer);

    /* repaint requested region of the screen */
    ui_element_draw (interface->root,
                     interface,
                     interface->repaint_region,
                     interface->window_region);

    /* clear the repaint region */
    interface->repaint_region = region_make (0, 0, 0, 0);

    /* present the image to the screen */
    SDL_SetRenderTarget (interface->renderer, NULL);
    SDL_RenderClear     (interface->renderer);
    SDL_RenderCopy      (interface->renderer, interface->framebuffer, NULL, NULL);
    SDL_RenderPresent   (interface->renderer);
}

void interface_repaint (interface_t *interface, region_t region) {

    /* TODO: instead of consolodating here,
     * queue them all up and consolodate at render time
     * for performance ??? */

    /* here we queue up a repaint request for the given region */

    /* consolodate this request with existing requests */
    interface->repaint_region = region_bounds (interface->repaint_region,
                                               region);
}

int interface_process (interface_t *interface) {

    SDL_Event event;

    while (SDL_PollEvent (&event)) {

        switch (event.type) {

            case SDL_WINDOWEVENT:

                switch (event.window.event) {

                    case SDL_WINDOWEVENT_SHOWN:
                        interface_repaint (interface, interface->window_region);
                        break;

                    case SDL_WINDOWEVENT_EXPOSED:
                        /* TODO only redraw damaged regions */
                        interface_repaint (interface, interface->window_region);
                        break;

                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        interface_update_window_region (interface);
                        interface_repaint (interface, interface->window_region);
                        break;
                }

                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:

                switch (event.key.keysym.sym) {

                    case SDLK_ESCAPE:
                        interface->running = false;
                        return EXIT_SUCCESS;

                    default:
                        {
                            ui_event_t ui_event
                                = ui_event_make_keyboard (event.key.keysym.sym,
                                                          event.key.state == SDL_PRESSED);
                            ui_element_event (interface->root, interface, ui_event, interface->window_region);
                        }
                        break;
                }

                break;

            case SDL_TEXTINPUT:
                /* TODO */
                break;

            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                {
                    int x, y;
                    ui_event_t ui_event;
                    SDL_GetMouseState (&x, &y);
                    ui_event
                        = ui_event_make_mouse (event.type == SDL_MOUSEMOTION
                                                           ? EVENT_MOUSE_MOVE
                                                           : EVENT_MOUSE_BUTTON,
                                               vec2_make (x, y),
                                               event.button.button,
                                               event.button.state == SDL_PRESSED);
                    ui_element_event (interface->root, interface, ui_event, interface->window_region);
                }
                break;

            case SDL_QUIT:
                interface->running = false;
                return EXIT_SUCCESS;

            default:
                break;
        }
    }

    return EXIT_SUCCESS;
}

int interface_run (interface_t *interface) {

    int code = EXIT_SUCCESS;

    while (interface->running) {

        interface_render (interface);
        code = interface_process (interface);
    }

    return code;
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
