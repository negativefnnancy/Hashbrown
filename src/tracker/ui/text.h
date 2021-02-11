#ifndef UI_TEXT_H
#define UI_TEXT_H

#include "box.h"
#include "../color.h"

typedef enum text_alignment_t {

    TEXT_ALIGNMENT_LEFT,
    TEXT_ALIGNMENT_RIGHT,
    TEXT_ALIGNMENT_CENTER,

    N_TEXT_ALIGNMENT

} text_alignment_t;

typedef struct ui_text_t {

    ui_box_t base;
    color_t color_foreground;
    color_t color_background;
    color_t color_hovered;
    color_t color_pressed;
    char *text;
    text_alignment_t alignment;

} ui_text_t;

void ui_text_init (ui_text_t *element,
                   ui_box_style_t style,
                   color_t color_foreground,
                   color_t color_background,
                   color_t color_hovered,
                   color_t color_pressed,
                   char *text,
                   text_alignment_t alignment);

void ui_text_draw (ui_text_t *element,
                   struct interface_t *interface,
                   region_t clipping_region,
                   region_t element_region);

void ui_text_event (ui_text_t *element,
                    struct interface_t *interface,
                    ui_event_t event,
                    region_t element_region);

ui_text_t *ui_text_create  (ui_box_style_t style,
                            color_t color_foreground,
                            color_t color_background,
                            color_t color_hovered,
                            color_t color_pressed,
                            char *text,
                            text_alignment_t alignment);
void       ui_text_destroy (ui_text_t *element);

#endif /* UI_TEXT_H */
