#include <stdlib.h> // For malloc, free
#include <string.h> // For memset
#include "table.h"

Table *table_create(size_t rows, size_t cols) {
    Table *table = malloc(sizeof(Table));
    if (table == NULL) {
        return NULL;
    }
    table->rows = rows;
    table->cols = cols;
    table->cells = malloc(rows * cols * sizeof(Cell));
    if (table->cells == NULL) {
        free(table);
        return NULL;
    }
    // Initialize cells to zero (CELL_TYPE_TEXT, empty String_View)
    memset(table->cells, 0, rows * cols * sizeof(Cell));
    return table;
}

void table_free(Table *table) {
    if (table != NULL) {
        // TODO: Free any dynamically allocated content within cells (e.g., Expr*)
        free(table->cells);
        free(table);
    }
}

Cell *table_get_cell(Table *table, size_t row, size_t col) {
    if (table == NULL || row >= table->rows || col >= table->cols) {
        return NULL; // Out of bounds or invalid table
    }
    return &table->cells[row * table->cols + col];
}
