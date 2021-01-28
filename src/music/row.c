#include <hashbrown/music/row.h>

void row_init (row_t *row, uint8_t note, uint8_t volume, uint16_t effect) {

    row->note   = note;
    row->volume = volume;
    row->effect = effect;
}

