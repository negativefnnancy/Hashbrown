#include <stdlib.h>

#include <hashbrown/music/pattern.h>

void pattern_init (pattern_t *pattern, size_t n_rows) {

    pattern->rows = calloc (n_rows, sizeof (row_t));
    pattern->n_rows = n_rows;
}

void pattern_deinit (pattern_t *pattern) {

    free (pattern->rows);
}
