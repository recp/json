/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_impl_mem_h
#define json_impl_mem_h

#include "../common.h"
#include <stdlib.h>
#include <string.h>

typedef struct json_mem_t {
  struct json_mem_t *next;
  size_t             size;
  size_t             capacity;
  char               data[];
} json_mem_t;

#define JSON_MEM_PAGE (32768 - sizeof(json_mem_t)) /* 32K */

JSON_INLINE
void*
json__impl_calloc(json_doc_t * __restrict doc, size_t size) {
  void       *data;
  json_mem_t *mem;
  size_t      sizeToAlloc;
  
  mem         = doc->memroot;
  sizeToAlloc = (JSON_MEM_PAGE < size) ? size : JSON_MEM_PAGE;
  
  if (mem->capacity < (mem->size + size)) {
    mem           = calloc(1, sizeof(*mem) + sizeToAlloc);
    mem->capacity = sizeToAlloc;
    mem->next     = doc->memroot;
    doc->memroot  = mem;
  }
  
  data       = (char *)mem->data + mem->size;
  mem->size += size;

  return data;
}

#endif /* json_impl_mem_h */
