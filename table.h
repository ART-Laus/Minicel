#ifndef TABLE_H_
#define TABLE_H_

#include <stddef.h> // For size_t
#include "cell.h"   // For Cell type

typedef struct {
    Cell *cells;
    size_t rows;
    size_t cols;
} Table;

// Function to create a new table
Table *table_create(size_t rows, size_t cols);

// Function to free table memory
void table_free(Table *table);

// Function to get a cell with bounds checking
// Returns NULL if out of bounds
Cell *table_get_cell(Table *table, size_t row, size_t col);

#endif // TABLE_H_
