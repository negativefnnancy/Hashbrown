#ifndef PATTERN_H
#define PATTERN_H

#include "row.h"

typedef struct pattern_t {

    size_t n_rows;
    row_t *rows;

} pattern_t;

void pattern_init   (pattern_t *pattern, size_t n_rows);
void pattern_deinit (pattern_t *pattern);

#endif /* PATTERN_H */
