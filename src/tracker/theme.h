#ifndef THEME_H
#define THEME_H

#include "color.h"

#define N_THEME_COLORS 5

typedef struct theme_t {

    color_t colors[N_THEME_COLORS];
    /* TODO sizes n stuff maybe */

} theme_t;
 
extern theme_t default_theme;

/* TODO theme loading from config files n stuff */

#endif /* THEME_H */
