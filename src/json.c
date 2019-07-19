/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/json/json.h"
#include "mem.h"

JSON_INLINE
const void*
json_key(const char ** __restrict ptr, int * __restrict keysize) {
  const char *pi, *end, *start;
  char        c;

  pi = *ptr;
  c  = *pi;

  while (c == '\"' || c == '\'' || c == ' ' || c == ',')
    c = *++pi;

  start = end = pi;

  while (c != ':' && c != '\"' && c != '\'' && c != '\0') {
    if (c != ' '
        && c != '\r'
        && c != '\n'
        && c != '\t')
      end = pi + 1;
    
    c = *++pi;
  }

  *keysize = (int)(end - start);
  *ptr     = pi;

  return start;
}

JSON_INLINE
const void*
json_value(const char ** __restrict ptr, int * __restrict valuesize) {
  const char *pi, *end, *start;
  char        c;

  pi  = *ptr;
  c   = *pi;

  while (c == ':' || c == '\"' || c == '\'' || c == ' ')
    c = *++pi;

  start = end = pi;
  
  while (c != '\"'
         && c != '\''
         && c != ','
         && c != '{'
         && c != '}'
         && c != '['
         && c != ']'
         && c != '\0') {
    if (c != ' '
        && c != '\r'
        && c != '\n'
        && c != '\t')
      end = pi + 1;
    
    c = *++pi;
  }

  *valuesize = (int)(end - start);
  *ptr       = pi;

  return start;
}

JSON_EXPORT
json_doc_t*
json_parse(const char * __restrict contents) {
  json_doc_t *doc;
  json_t     *obj, *val, *parent;
  const char *key;
  json_t      tmproot;
  int         keysize;
  char        c;
  bool        lookingForKey;

  if (!contents || (c = *contents) == '\0')
    return NULL;

  doc            = calloc(1, sizeof(*doc));
  doc->memroot   = calloc(1, sizeof(json_mem_t) + JSON_MEM_PAGE);
  doc->ptr       = contents;
  
  tmproot.type   = JSON_OBJECT;
  tmproot.prev   = NULL;
  tmproot.value  = NULL;
  
  key            = NULL;
  parent         = &tmproot;
  obj            = parent;
  keysize        = 0;
  lookingForKey  = false;

  ((json_mem_t *)doc->memroot)->capacity = sizeof(json_mem_t) + JSON_MEM_PAGE;
  
  do {
  again:
    /* child */
    switch (c) {
      /* trim */
      case ' ':
      case '\r':
      case '\n':
      case '\t':
      case '\'':
      case '"':
        break;
      case '{':
      case '[': {
        if (obj->type == JSON_ARRAY)
          ++((json_array_t *)obj)->count;

        /* switch parent */
        parent = obj;

        if (c == '{') {
          obj           = json_calloc(doc, sizeof(json_t));
          obj->type     = JSON_OBJECT;
          lookingForKey = true;
        } else {
          obj       = json_calloc(doc, sizeof(json_array_t));
          obj->type = JSON_ARRAY;
        }

        /* parent must not be NULL */
        obj->prev     = parent;
        obj->next     = parent->value;
        parent->value = obj;

        if (key) {
          obj->key     = key;
          obj->keySize = keysize;
          key          = NULL;
        }

        break;
      }
      case '}':
      case ']':
        /* switch parent back */
        if (!parent || !parent->prev)
          goto err;

        obj           = parent;
        parent        = parent->prev;
        lookingForKey = obj->type == JSON_OBJECT;

        break;
      case ',': {
        lookingForKey = obj->type == JSON_OBJECT;
        break;
      }
      default: {
        /* looking for key */
        if (lookingForKey) {
          key = json_key(&doc->ptr, &keysize);
          if (key == NULL || ((c = *key) == '\0'))
            goto err;
          
          lookingForKey = false;
          c             = *doc->ptr;

          /* jump to value */
          for (;;) {
            switch (c) {
              case ' ':
              case '\t':
              case '\n':
              case '\r':
              case '"':
              case '\'':
              case ':':
                c = *++doc->ptr;
                continue;
              default:
                goto val;
            }
          }

        val:
          goto again;
        }

        /* looking for value */
        else {
          val       = json_calloc(doc, sizeof(json_t));
          val->type = JSON_STRING;

          if (obj->type == JSON_ARRAY)
            ++((json_array_t *)obj)->count;

          /* parent must not be NULL */
          val->prev  = obj;
          val->next  = obj->value;
          obj->value = val;

          if (key) {
            val->key     = key;
            val->keySize = keysize;
            key          = NULL;
          }

          val->value = json_value(&doc->ptr, &val->valSize);
          c          = *doc->ptr;

          goto again;
        } /* if lookingForKey */
      } /* switch->default */
    } /* switch */
  } while ((c = *doc->ptr) != '\0' && (c = *++doc->ptr) != '\0');

err:
  if (tmproot.value)
    ((json_t *)tmproot.value)->prev = NULL;
  
  doc->root = tmproot.value;
  return doc;
}

JSON_EXPORT
void
json_free(json_doc_t * __restrict jsondoc) {
  json_mem_t *mem, *tofree;

  mem = jsondoc->memroot;
  while (mem) {
    tofree = mem;
    mem    = mem->next;
    free(tofree);
  }

  free(jsondoc);
}
