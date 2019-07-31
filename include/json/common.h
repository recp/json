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
  int            valSize;
  int            keySize;
  json_type_t    type;
} json_t;

typedef struct json_array_t {
  json_t base;
  int    count;
} json_array_t;

typedef struct json_doc_t {
  void         *memroot;
  const json_t *root;
  const char   *ptr;
} json_doc_t;

#endif /* json_common_h */
