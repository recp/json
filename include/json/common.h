/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_common_h
#define json_common_h

#define _USE_MATH_DEFINES /* for windows */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#if defined(_MSC_VER)
#  ifdef JSON_DLL
#    define JSON_EXPORT __declspec(dllexport)
#  else
#    define JSON_EXPORT __declspec(dllimport)
#  endif
#  define JSON_INLINE __forceinline
#else
#  define JSON_EXPORT __attribute__((visibility("default")))
#  define JSON_INLINE static inline __attribute((always_inline))
#endif

#define JSON_ARR_LEN(ARR) (sizeof(ARR)/sizeof(ARR[0]))

typedef enum json_type_t {
  JSON_UNKOWN  = 0,
  JSON_OBJECT  = 1,
  JSON_ARRAY   = 2,
  JSON_STRING  = 3,

  /*
  JSON_BOOL    = 5,
  JSON_NUMBER  = 4,

  JSON_INTEGER = 6,
  JSON_FLOAT   = 7,
  JSON_NULL    = 8
   */
} json_type_t;

typedef struct json_t {
  struct json_t *parent;
  struct json_t *next;
  const char    *key;
  void          *value;
  int            valsize;
  int            keysize;
  json_type_t    type;
} json_t;

typedef struct json_array_t {
  json_t base;
  int    count;
} json_array_t;

typedef struct json_doc_t {
  void       *memroot;
  json_t     *root;
  const char *ptr;
} json_doc_t;

JSON_INLINE
bool
json__ascii_space(char c) {
  return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

JSON_INLINE
bool
json__value_end(char c) {
  return c == ','
      || c == '{'
      || c == '}'
      || c == '['
      || c == ']';
}

JSON_INLINE
const char *
json__rtrim_ascii(const char * __restrict begin,
                  const char * __restrict end) {
  while (end > begin && json__ascii_space(end[-1]))
    end--;

  return end;
}

JSON_INLINE
bool
json__quote_escaped(const char * __restrict begin,
                    const char * __restrict quote) {
  const char *p;
  size_t      count;

  p     = quote;
  count = 0;

  while (p > begin && p[-1] == '\\') {
    p--;
    count++;
  }

  return (count & 1u) != 0;
}

JSON_INLINE
const char *
json__find_quote(const char * __restrict p, char quote) {
  const char *found;

  while ((found = strchr(p, quote))) {
    if (!json__quote_escaped(p, found))
      return found;

    p = found + 1;
  }

  return NULL;
}

JSON_INLINE
const char *
json__find_quote_len(const char * __restrict p,
                     const char * __restrict end,
                     char                    quote) {
  const char *found;

  while (p < end && (found = memchr(p, quote, (size_t)(end - p)))) {
    if (!json__quote_escaped(p, found))
      return found;

    p = found + 1;
  }

  return NULL;
}

#endif /* json_common_h */
