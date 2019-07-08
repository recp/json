/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/json.h"
#include "mem.h"
#include <stdlib.h>
#include <string.h>

JSON_INLINE
const char*
json_key(const char ** __restrict ptr) {
  const char *pi;
  char        c;

  pi = *ptr;
  c  = *pi;

  while (c == '\"' || c == '\'' || c == ' ') {
    c = *++pi;
  }

  *ptr = pi;

  return pi;
}

JSON_INLINE
uintptr_t
json_keysize(const char ** __restrict ptr) {
  const char *p, *pi, *end;
  char        c;

  pi = *ptr;
  c  = *pi;
  p   = pi;
  end = pi;

  while (c != ':' && c != '\"' && c != '\'') {
    if (c != ' ')
      end = pi + 1;

    c = *++pi;
  }

  *ptr = pi;

  return end - p;
}

JSON_INLINE
uintptr_t
json_valsize(const char ** __restrict ptr) {
  const char *p, *pi, *end;
  char        c;
  
  pi  = *ptr;
  c   = *pi;
  p   = pi;
  end = pi;
  
  while (c != '\"'
         && c != '\''
         && c != ','
         && c != '{'
         && c != '}'
         && c != '['
         && c != ']') {
    if (c != ' ')
      end = pi + 1;

    c = *++pi;
  }
  
  *ptr = pi;
  
  return end - p;
}

JSON_INLINE
const char*
json_value(const char ** __restrict ptr) {
  const char *pi;
  char        c;

  pi = *ptr;
  c  = *pi;

  while (c == ':' || c == '\"' || c == '\"' || c == ' ') {
    c = *++pi;
  }

  *ptr = pi;

  return pi;
}

json_t*
json_parse(const char * __restrict contents) {
  json_doc_t *doc;
  json_t     *object;
  const char *ptr;
  char        c;

  doc          = calloc(1, sizeof(*doc));
  doc->memroot = calloc(1, JSON_MEM_PAGE);
  ptr          = contents;
  object       = NULL;
  
  while ((c = *ptr++) != '\0') {
    switch (c) {
      case '{':
        object          = json_calloc(doc, sizeof(json_t));
        object->key     = json_key(&ptr);
        object->keySize = json_keysize(&ptr);
        object->value   = json_value(&ptr);
        object->valSize = json_valsize(&ptr);
        break;
      case '[':
        object = json_calloc(doc, sizeof(json_array_t));
        break;
    }
  }
  
  return object;
}

json_t*
json_get(json_t * __restrict object, const char * __restrict key) {
  return NULL;
}

void
json_set(json_t * __restrict object, const char * __restrict key) {
  
}
