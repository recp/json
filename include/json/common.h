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
const char *
json__skip_ascii_space_len(const char * __restrict p,
                           const char * __restrict end) {
  const uint64_t spaces = 0x2020202020202020ull;

  while (p < end && json__ascii_space(*p)) {
    if ((size_t)(end - p) >= sizeof(uint64_t)) {
      uint64_t v;

      memcpy(&v, p, sizeof(v));
      if (v == spaces) {
        p += sizeof(v);
        continue;
      }
    }

    p++;
  }

  return p;
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
bool
json__has_zero_byte8(uint64_t v) {
  return ((v - 0x0101010101010101ull) & ~v & 0x8080808080808080ull) != 0;
}

JSON_INLINE
bool
json__has_value_end8(uint64_t v) {
  return json__has_zero_byte8(v ^ 0x2c2c2c2c2c2c2c2cull)
      || json__has_zero_byte8(v ^ 0x7b7b7b7b7b7b7b7bull)
      || json__has_zero_byte8(v ^ 0x7d7d7d7d7d7d7d7dull)
      || json__has_zero_byte8(v ^ 0x5b5b5b5b5b5b5b5bull)
      || json__has_zero_byte8(v ^ 0x5d5d5d5d5d5d5d5dull);
}

JSON_INLINE
const char *
json__find_value_end_len(const char * __restrict p,
                         const char * __restrict end) {
  while ((size_t)(end - p) >= sizeof(uint64_t)) {
    uint64_t v;

    memcpy(&v, p, sizeof(v));
    if (json__has_value_end8(v))
      break;

    p += sizeof(v);
  }

  while (p < end && !json__value_end(*p))
    p++;

  return p < end ? p : NULL;
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
