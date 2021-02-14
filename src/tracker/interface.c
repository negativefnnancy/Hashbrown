#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interface.h"
#include "ui/event.h"

ui_element_t *interface_construct_row (interface_t *interface,
                                       size_t i_pattern,
                                       size_t i_row) {

    ui_box_style_t style_1, style_2;

    ui_layout_t *layout;
    ui_element_t **children;
    double *lengths;

    style_1 = ui_box_style_make (0,
                                 0,
                                 0, 
                                 interface->theme.colors[0],
                                 interface->theme.colors[0],
                                 interface->theme.colors[0]);
    style_2 = ui_box_style_make (0,
                                 0,
                                 0, 
                                 interface->theme.colors[0],
                                 interface->theme.colors[0],
                                 interface->theme.colors[0]);

    children = calloc (4, sizeof (ui_element_t *));
    children[0] = (ui_element_t *) ui_text_create (style_2,
                                                   interface->theme.colors[2],
                                                   interface->theme.colors[0],
                                                   interface->theme.colors[1],
                                                   interface->theme.colors[3],
                                                   malloc (2),
                                                   TEXT_ALIGNMENT_CENTER);
    children[1] = (ui_element_t *) ui_text_create (style_2,
                                                   interface->theme.colors[2],
                                                   interface->theme.colors[0],
                                                   interface->theme.colors[1],
                                                   interface->theme.colors[3],
                                                   malloc (4),
                                                   TEXT_ALIGNMENT_CENTER);
    children[2] = (ui_element_t *) ui_text_create (style_2,
                                                   interface->theme.colors[2],
                                                   interface->theme.colors[0],
                                                   interface->theme.colors[1],
                                                   interface->theme.colors[3],
                                                   malloc (3),
                                                   TEXT_ALIGNMENT_CENTER);
    children[3] = (ui_element_t *) ui_text_create (style_2,
                                                   interface->theme.colors[2],
                                                   interface->theme.colors[0],
                                                   interface->theme.colors[1],
                                                   interface->theme.colors[3],
                                                   malloc (5),
                                                   TEXT_ALIGNMENT_CENTER);

    strcpy (((ui_text_t *) children[0])->text, "+");
    strcpy (((ui_text_t *) children[1])->text, "...");
    strcpy (((ui_text_t *) children[2])->text, "..");
    strcpy (((ui_text_t *) children[3])->text, "....");

    lengths = calloc (4, sizeof (double));
    lengths[0] = 8;
    lengths[1] = 8 * 3;
    lengths[2] = 8 * 2;
    lengths[3] = 8 * 4;

    layout = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_HORIZONTAL,
                                                     8,
                                                     lengths);

    return (ui_element_t *) ui_container_create (style_1, layout, 4, children);
}

ui_element_t *interface_construct_channel_view (interface_t *interface, size_t i_channel) {

    ui_box_style_t style_1, style_2, style_3;
    size_t i;

    ui_layout_t *layout;
    ui_element_t **children;
    double *lengths;

    ui_layout_t *layout_header;
    ui_element_t **children_header;
    double *lengths_header;

    ui_layout_t *layout_rows;
    ui_element_t **children_rows;
    double *lengths_rows;

    size_t n_rows =
        interface->module->patterns[interface->module->channels[i_channel].i_patterns[interface->player.positions[i_channel].i_i_pattern]].n_rows;

    style_1 = ui_box_style_make (0,
                                 0,
                                 0, 
                                 interface->theme.colors[0],
                                 interface->theme.colors[0],
                                 interface->theme.colors[2]);
    style_2 = ui_box_style_make (0,
                                 0,
                                 0, 
                                 interface->theme.colors[0],
                                 interface->theme.colors[0],
                                 interface->theme.colors[0]);
    style_3 = ui_box_style_make (0,
                                 0,
                                 8, 
                                 interface->theme.colors[0],
                                 interface->theme.colors[0],
                                 interface->theme.colors[0]);

    children_header = calloc (10, sizeof (ui_element_t *));
    children_header[0] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[2],
                                                          malloc (3),
                                                          TEXT_ALIGNMENT_CENTER);
    sprintf (((ui_text_t *) children_header[0])->text, "%d", i_channel + 1);
    children_header[1] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[1],
                                                          interface->theme.colors[3],
                                                          "-",
                                                          TEXT_ALIGNMENT_CENTER);
    children_header[2] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[0],
                                                          malloc (4),
                                                          TEXT_ALIGNMENT_CENTER);
    sprintf (((ui_text_t *) children_header[2])->text, "%d", interface->player.positions[i_channel].i_i_pattern + 1);
    children_header[3] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[1],
                                                          interface->theme.colors[3],
                                                          "+",
                                                          TEXT_ALIGNMENT_CENTER);
    children_header[4] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[1],
                                                          interface->theme.colors[3],
                                                          "-",
                                                          TEXT_ALIGNMENT_CENTER);
    children_header[5] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[0],
                                                          malloc (4),
                                                          TEXT_ALIGNMENT_CENTER);
    sprintf (((ui_text_t *) children_header[5])->text, "%d",
            interface->module->channels[i_channel].i_patterns[interface->player.positions[i_channel].i_i_pattern] + 1);
    children_header[6] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[1],
                                                          interface->theme.colors[3],
                                                          "+",
                                                          TEXT_ALIGNMENT_CENTER);
    children_header[7] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[1],
                                                          interface->theme.colors[3],
                                                          "-",
                                                          TEXT_ALIGNMENT_CENTER);
    children_header[8] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[0],
                                                          malloc (4),
                                                          TEXT_ALIGNMENT_CENTER);
    sprintf (((ui_text_t *) children_header[8])->text, "%d", n_rows);
    children_header[9] = (ui_element_t *) ui_text_create (style_2,
                                                          interface->theme.colors[2],
                                                          interface->theme.colors[0],
                                                          interface->theme.colors[1],
                                                          interface->theme.colors[3],
                                                          "+",
                                                          TEXT_ALIGNMENT_CENTER);

    lengths_header = calloc (10, sizeof (double));
    lengths_header[0] = 14;
    lengths_header[1] = 14;
    lengths_header[2] = 10;
    lengths_header[3] = 14;
    lengths_header[4] = 14;
    lengths_header[5] = 10;
    lengths_header[6] = 14;
    lengths_header[7] = 14;
    lengths_header[8] = 14;
    lengths_header[9] = 14;

    layout_header = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_HORIZONTAL,
                                                            0,
                                                            lengths_header);

    children_rows = calloc (n_rows, sizeof (ui_element_t *));
    for (i = 0; i < n_rows; i++)
        children_rows[i] = interface_construct_row (interface, 0, i);

    lengths_rows = calloc (n_rows, sizeof (double));
    for (i = 0; i < n_rows; i++)
        lengths_rows[i] = 14;

    layout_rows = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_VERTICAL,
                                                          0,
                                                          lengths_rows);

    children = calloc (interface->module->n_channels, sizeof (ui_element_t *));
    children[0] = (ui_element_t *) ui_container_create (style_2, layout_header, 10, children_header);
    children[1] = (ui_element_t *) ui_container_create (style_3,
                                                        layout_rows,
                                                        n_rows,
                                                        children_rows);

    lengths = calloc (interface->module->n_channels, sizeof (double));
    lengths[0] = 14;
    lengths[1] = 0;

    layout = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_VERTICAL,
                                                     1,
                                                     lengths);

    return (ui_element_t *) ui_container_create (style_1, layout, 2, children);
}

void interface_construct_pattern_view (interface_t *interface) {

    ui_box_style_t style;

    size_t i;
    ui_layout_t *layout;
    ui_element_t **children;
    double *lengths;

    if (interface->ui.view_pattern) {

        /* TODO free old stuff */
    }

    style = ui_box_style_make (0,
                               1,
                               0, 
                               interface->theme.colors[0],
                               interface->theme.colors[2],
                               interface->theme.colors[2]);

    children = calloc (interface->module->n_channels, sizeof (ui_element_t *));
    lengths  = calloc (interface->module->n_channels, sizeof (double));

    for (i = 0; i < interface->module->n_channels; i++) {

        children[i] = interface_construct_channel_view (interface, i);
        lengths[i] = 132;
    }

    layout = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_HORIZONTAL,
                                                     1,
                                                     lengths);

    interface->ui.view_pattern = ui_container_create (style,
                                                      layout,
                                                      interface->module->n_channels,
                                                      children);
}

void interface_construct (interface_t *interface) {

    ui_box_style_t style_1, style_2, style_3, style_4, style_5, style_6;

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
    style_4 = ui_box_style_make (0,
                                 0,
                                 0,
                                 interface->theme.colors[0],
                                 interface->theme.colors[0],
                                 interface->theme.colors[0]);
    style_5 = ui_box_style_make (0,
                                 1,
                                 0,
                                 interface->theme.colors[0],
                                 interface->theme.colors[2],
                                 interface->theme.colors[0]);
    style_6 = ui_box_style_make (0,
                                 1,
                                 2,
                                 interface->theme.colors[0],
                                 interface->theme.colors[2],
                                 interface->theme.colors[0]);

    interface->ui.button_save       = ui_text_create (style_3,
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[1],
                                                      interface->theme.colors[3],
                                                      "SAVE",
                                                      TEXT_ALIGNMENT_CENTER);
    interface->ui.button_load       = ui_text_create (style_3,
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[1],
                                                      interface->theme.colors[3],
                                                      "LOAD",
                                                      TEXT_ALIGNMENT_CENTER);
    interface->ui.button_render     = ui_text_create (style_3,
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[1],
                                                      interface->theme.colors[3],
                                                      "RENDER",
                                                      TEXT_ALIGNMENT_CENTER);
    interface->ui.text_title        = ui_text_create (style_4,
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      "TITLE:",
                                                      TEXT_ALIGNMENT_LEFT);
    interface->ui.text_description  = ui_text_create (style_4,
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      "DESCRIPTION:",
                                                      TEXT_ALIGNMENT_LEFT);
    interface->ui.text_composer     = ui_text_create (style_4,
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      "COMPOSER:",
                                                      TEXT_ALIGNMENT_LEFT);
    interface->ui.field_title       = ui_text_create (style_5,
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->module->title,
                                                      TEXT_ALIGNMENT_RIGHT);
    interface->ui.field_description = ui_text_create (style_5,
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->module->description,
                                                      TEXT_ALIGNMENT_RIGHT);
    interface->ui.field_composer    = ui_text_create (style_5,
                                                      interface->theme.colors[2],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->theme.colors[0],
                                                      interface->module->composer,
                                                      TEXT_ALIGNMENT_RIGHT);

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
    interface->ui.layout_5
        = (ui_layout_t *) ui_layout_split_create (SPLIT_ORIENTATION_VERTICAL,
                                                  interface->theme.spacing,
                                                  interface->ui.lengths_5);

    interface_construct_pattern_view (interface);
    interface->ui.view_configuration = ui_dummy_create (interface->theme.colors[0], style_2);
    interface->ui.view_metadata      = ui_container_create (style_6,
                                                            interface->ui.layout_5,
                                                            6,
                                                            interface->ui.children_5);
    interface->ui.view_envelope      = ui_dummy_create (interface->theme.colors[0], style_2);

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

    interface->ui.lengths_1[0] = 140;
    interface->ui.lengths_1[1] = 0;

    interface->ui.lengths_2[0] = 0;
    interface->ui.lengths_2[1] = 140;

    interface->ui.lengths_3[0] = 134;
    interface->ui.lengths_3[1] = 0;
    interface->ui.lengths_3[2] = 24;

    interface->ui.lengths_4[0] = 38;
    interface->ui.lengths_4[1] = 38;
    interface->ui.lengths_4[2] = 0;

    interface->ui.lengths_5[0] = 18;
    interface->ui.lengths_5[1] = 18;
    interface->ui.lengths_5[2] = 18;
    interface->ui.lengths_5[3] = 18;
    interface->ui.lengths_5[4] = 18;
    interface->ui.lengths_5[5] = 18;

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

    interface->ui.children_5[0] = (ui_element_t *) interface->ui.text_title;
    interface->ui.children_5[1] = (ui_element_t *) interface->ui.field_title;
    interface->ui.children_5[2] = (ui_element_t *) interface->ui.text_composer;
    interface->ui.children_5[3] = (ui_element_t *) interface->ui.field_composer;
    interface->ui.children_5[4] = (ui_element_t *) interface->ui.text_description;
    interface->ui.children_5[5] = (ui_element_t *) interface->ui.field_description;

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
    soundchip_init (&interface->soundchip);
    player_init (&interface->player, module, &interface->soundchip);

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
