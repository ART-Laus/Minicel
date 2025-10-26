#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  for (size_t row = 0; input.count > 0; ++row) {
    String_View line = sv_chop_by_delim(&input, '\n');
    for (size_t col = 0; line.count > 0; ++col) {
      String_View cell = sv_trim(sv_chop_by_delim(&line, '|'));
      printf("%s:%zu:%zu: " SV_Fmt "\n", input_file_path, row + 1, col + 1,
             SV_Arg(cell));
    }
  }

  free(content);
  return 0;
}