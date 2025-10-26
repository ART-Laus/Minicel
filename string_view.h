#ifndef STRING_VIEW_H_
#define STRING_VIEW_H_

#include <ctype.h>
#include <stddef.h>
#include <string.h>

// -- Public Interface --

typedef struct {
  size_t count;
  const char *data;
} String_View;

#define SV_Fmt "%.*s"
#define SV_Arg(sv) (int)(sv).count, (sv).data

String_View sv_trim(String_View sv);
String_View sv_chop_by_delim(String_View *sv, char delim);

#endif // STRING_VIEW_H_

// -- Implementation --

#ifdef SV_IMPLEMENTATION

static String_View sv_trim_left(String_View sv) {
  size_t i = 0;
  while (i < sv.count && isspace(sv.data[i])) {
    i++;
  }
  sv.data += i;
  sv.count -= i;
  return sv;
}

static String_View sv_trim_right(String_View sv) {
  size_t i = 0;
  while (i < sv.count && isspace(sv.data[sv.count - 1 - i])) {
    i++;
  }
  sv.count -= i;
  return sv;
}

String_View sv_trim(String_View sv) { return sv_trim_right(sv_trim_left(sv)); }

String_View sv_chop_by_delim(String_View *sv, char delim) {
  const char *end = memchr(sv->data, delim, sv->count);
  String_View result;

  if (end != NULL) {
    result.count = end - sv->data;
    result.data = sv->data;
    sv->data = end + 1;
    sv->count -= result.count + 1;
  } else {
    result = *sv;
    sv->count = 0;
    sv->data += result.count;
  }

  return result;
}

#endif // SV_IMPLEMENTATION
