#ifndef PATTERN_H
#define PATTERN_H

#include "row.h"

typedef struct pattern_t {

    size_t n_rows;
    row_t *rows;

} pattern_t;

#endif /* PATTERN_H */
