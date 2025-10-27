#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cell.h"

#define SV_IMPLEMENTATION
#include "string_view.h"

// -- Application code --

void usage(FILE *stream) { fprintf(stream, "Usage: ./minicel <input.csv>\n"); }

char *slurp_file(const char *file_path, size_t *size) {
  char *buffer = NULL;
  FILE *f = fopen(file_path, "rb");
  if (f == NULL) goto error;

  if (fseek(f, 0, SEEK_END) < 0) goto error;
  long m = ftell(f);
  if (m < 0) goto error;

  buffer = malloc(m);
  if (buffer == NULL) goto error;

  if (fseek(f, 0, SEEK_SET) < 0) goto error;
  size_t n = fread(buffer, 1, m, f);
  if (ferror(f)) goto error;

  if (size) *size = n;
  fclose(f);
  return buffer;

error:
  if (f) fclose(f);
  if (buffer) free(buffer);
  return NULL;
}

void estimate_table_size(String_View content, size_t *rows, size_t *cols) {
  *rows = 0;
  *cols = 0;
  while (content.count > 0) {
    (*rows) += 1;
    String_View line = sv_chop_by_delim(&content, '\n');
    size_t n = 0;
    while (line.count > 0) {
      n += 1;
      sv_chop_by_delim(&line, '|');
    }
    if (n > *cols) *cols = n;
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    usage(stderr);
    fprintf(stderr, "ERROR: input file is not provided\n");
    exit(1);
  }

  const char *input_file_path = argv[1];
  size_t content_size = 0;
  char *content = slurp_file(input_file_path, &content_size);

  if (content == NULL) {
    fprintf(stderr, "ERROR: could not read file %s: %s\n", input_file_path,
            strerror(errno));
    exit(1);
  }

  String_View input = {.count = content_size, .data = content};

  size_t rows, cols;
  estimate_table_size(input, &rows, &cols);
  printf("Table size: %zu rows, %zu cols\n", rows, cols);

  // Allocate table
  Cell *table = malloc(rows * cols * sizeof(Cell));
  if (table == NULL) {
    fprintf(stderr, "ERROR: Not enough memory for a table of size %zu x %zu\n",
            rows, cols);
    exit(1);
  }

  // Reset input String_View as estimate_table_size consumes it
  input.count = content_size;
  input.data = content;

  // Parse and fill the table
  for (size_t row = 0; row < rows; ++row) {
    String_View line = sv_chop_by_delim(&input, '\n');
    for (size_t col = 0; col < cols; ++col) {
      String_View cell_sv;
      if (line.count > 0) {
        cell_sv = sv_trim(sv_chop_by_delim(&line, '|'));
      } else {
        // Handle ragged rows by filling with empty content
        cell_sv.count = 0;
        cell_sv.data = "";
      }
      table[row * cols + col].type = CELL_TYPE_TEXT;
      table[row * cols + col].as.text = cell_sv;
    }
  }

  // Print from memory to verify
  printf("\n--- Verifying content from memory table ---\n");
  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < cols; ++col) {
      switch (table[row * cols + col].type) {
      case CELL_TYPE_TEXT:
        printf("Cell[%zu, %zu]: " SV_Fmt "\n", row, col,
               SV_Arg(table[row * cols + col].as.text));
        break;
      case CELL_TYPE_NUMBER:
        printf("Cell[%zu, %zu]: %f\n", row, col,
               table[row * cols + col].as.number);
        break;
      case CELL_TYPE_EXPR:
        // For now, just indicate it's an expression
        printf("Cell[%zu, %zu]: <expression>\n", row, col);
        break;
      }
    }
  }

  free(table);
  free(content);
  return 0;
}
