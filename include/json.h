/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_h
#define json_h

#include "common.h"

typedef struct json_t {
  struct json_t *prev;
  struct json_t *next;
  struct json_t *child;
  const char    *key;
  const char    *value;
  size_t         keySize;
  size_t         valSize;
  bool           is_array:1;
  bool           is_string:1;
  bool           is_number:1;
} json_t;

typedef struct json_array_t {
  json_t   base;
  json_t **items;
  size_t   count;
} json_array_t;

typedef struct json_doc_t {
  void       *memroot;
  json_t     *root;
  const char *ptr;
} json_doc_t;

json_t*
json_parse(const char * __restrict contents);

json_t*
json_get(json_t * __restrict object, const char * __restrict key);

void
json_set(json_t * __restrict object, const char * __restrict key);

json_array_t*
json_array(json_t * __restrict object);

int32_t
json_int32(json_t * __restrict object);

int64_t
json_int64(json_t * __restrict object);

float
json_float(json_t * __restrict object);

double
json_double(json_t * __restrict object);

const char*
json_string(json_t * __restrict object);

#endif /* json_h */
