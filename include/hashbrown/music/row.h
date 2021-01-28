#ifndef ROW_H
#define ROW_H

#include <stdint.h>

typedef struct row_t {

    uint8_t note;       /* midi note number + 8th bit for gate control */
    uint8_t volume;     /* 2 x 4 bit stereo volume */
    uint16_t effect;    /* 16 bit effect value */

} row_t;

void row_init (row_t *row, uint8_t note, uint8_t volume, uint16_t effect);

#endif /* ROW_H */
