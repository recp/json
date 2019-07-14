/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_h
#define json_h

#include "common.h"

typedef enum json_type_t {
  JSON_UNKOWN = 0,
  JSON_OBJECT = 1,
  JSON_ARRAY  = 2,
  JSON_STRING = 3,
  JSON_NUMBER = 4,
  JSON_BOOL   = 5
} json_type_t;

typedef struct json_t {
  const struct json_t *prev;
  const struct json_t *next;
  const char          *key;
  const void          *value;
  int                  valSize;
  int                  keySize;
  json_type_t          type;
} json_t;

typedef struct json_array_t {
  json_t   base;
  int      count;
  json_t **items;
} json_array_t;

typedef struct json_doc_t {
  void         *memroot;
  const json_t *root;
  const char   *ptr;
} json_doc_t;

json_doc_t*
json_parse(const char * __restrict contents);

void
json_free(json_doc_t * __restrict jsondoc);

const json_t*
json_get(const json_t * __restrict object, const char * __restrict key);

void
json_set(json_t     * __restrict object,
         const char * __restrict key,
         void       * __restrict value);

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
