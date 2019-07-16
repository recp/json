/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_mem_h
#define json_mem_h

#include "../include/json/common.h"
#include <stdlib.h>
#include <string.h>

#define JSON_MEM_PAGE 8192

typedef struct json_mem_t {
  struct json_mem_t *next;
  size_t             size;
  char               data[];
} json_mem_t;


JSON_INLINE
void*
json_calloc(json_doc_t * __restrict doc, size_t size) {
  void       *data;
  json_mem_t *mem;
  
  mem = doc->memroot;
  
  if (mem->size > (JSON_MEM_PAGE - size)) {
    mem          = calloc(1, JSON_MEM_PAGE);
    mem->next    = doc->memroot;
    doc->memroot = mem;
  }
  
  data       = &mem->data[mem->size];
  mem->size += size;
  
  return data;
}

#endif /* json_mem_h */
