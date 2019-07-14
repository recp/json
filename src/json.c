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
#include <stdio.h>
#include <assert.h>

JSON_INLINE
const void*
json_key(const char ** __restrict ptr, int * __restrict keysize) {
  const char *pi, *end, *start;
  char        c;

  pi = *ptr;
  c  = *pi;

  while (c == '\"' || c == '\'' || c == ' ' || c == ',') {
    c = *++pi;
  }

  start = end = pi;

  while (c != ':' && c != '\"' && c != '\'' && c != '\0') {
    if (c != ' ')
      end = pi + 1;
    
    c = *++pi;
  }

  *keysize = (int)(end - start);

  *ptr = pi;
  return start;
}

JSON_INLINE
const void*
json_value(const char ** __restrict ptr, int * restrict valuesize) {
  const char *pi, *end, *start;
  char        c;

  pi  = *ptr;
  c   = *pi;

  while (c == ':' || c == '\"' || c == '\"' || c == ' ') {
    c = *++pi;
  }

  start = end = pi;
  
  while (c != '\"'
         && c != '\''
         && c != ','
         && c != '{'
         && c != '}'
         && c != '['
         && c != ']'
         && c != '\0') {
    if (c != ' ')
      end = pi + 1;
    
    c = *++pi;
  }

  *valuesize = (int)(end - start);

  *ptr = pi;

  return start;
}

void
json_child(json_doc_t  * __restrict doc,
           json_t      * __restrict parent) {
  json_t     *obj, *val, *lastval;
  const char *key;
  int         keysize;
  char        c;
  bool        lookingForKey, objIsStarted;

  if (!doc->ptr || (c = *doc->ptr) == '\0')
    return;

  lastval        = NULL;
  key            = NULL;
  obj            = parent;
  keysize        = 0;
  lookingForKey  = false;
  objIsStarted   = false;

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
        continue;
      case '{':
      case '[': {
        /* switch parent */
        parent = obj;
        obj    = json_calloc(doc, sizeof(json_t));

        if (c == '{') {
          obj->type     = JSON_OBJECT;
          lookingForKey = true;
        } else {
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

        lastval      = NULL;
        objIsStarted = true;
        break;
      }
      case '}':
      case ']':
        if (!objIsStarted)
          goto err;

        /* switch parent back */
        obj          = parent;
        objIsStarted = false;
        break;
      case ',': {
        lookingForKey = obj->type == JSON_OBJECT;
        continue;
      }
      default: {
        /* looking for key */
        if (lookingForKey) {
          key = json_key(&doc->ptr, &keysize);
          if (key == NULL || ((c = *key) == '\0'))
            goto err;
          lookingForKey = false;
          lastval = NULL;

          c = *doc->ptr;

          /* jump to value */
          do {
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
          } while (true);
          
        val:
          goto again;
        }

        /* looking for value */
        else {
          val       = json_calloc(doc, sizeof(json_t));
          val->type = JSON_STRING;

          /* for arrays */
          if (!lastval) {
            val->prev  = obj;
            obj->value = val;
          } else {
            lastval->next = val;
            val->prev     = lastval;
          }

          lastval = val;

          if (key) {
            val->key     = key;
            val->keySize = keysize;
            key          = NULL;
          }
          
          val->value = json_value(&doc->ptr, &val->valSize);
          c = *doc->ptr;
          goto again;

        } /* if lookingForKey */
      } /* switch->default */
    }
  } while ((c = *doc->ptr) != '\0' && (c = *++doc->ptr) != '\0');

err:
ret:
  return ;
}

json_doc_t*
json_parse(const char * __restrict contents) {
  json_doc_t *doc;
  json_t      tmproot;

  doc           = calloc(1, sizeof(*doc));
  doc->memroot  = calloc(1, JSON_MEM_PAGE);
  doc->ptr      = contents;
  tmproot.prev  = NULL;
  tmproot.value = NULL;

  json_child(doc, &tmproot);

  if (tmproot.value) {
    ((json_t *)tmproot.value)->prev = NULL;
  }

  doc->root = tmproot.value;
  return doc;
}

void
json_free(json_doc_t * __restrict jsondoc) {
  free(jsondoc->memroot);
  free(jsondoc);
}

const json_t*
json_get(const json_t * __restrict object, const char * __restrict key) {
  const json_t *iter;
  int           keysize;

  if (!object || !key || !(iter = object->value))
    return NULL;

  keysize = (int)strlen(key);
  while (iter && strncmp(iter->key, key, keysize) != 0)
    iter = iter->next;
  
  return iter;
}
