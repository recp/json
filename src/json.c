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
const char*
json_key(const char ** __restrict ptr, size_t * __restrict keysize) {
  const char *pi, *end, *start;
  char        c;

  pi = *ptr;
  c  = *pi;

  while (c == '\"' || c == '\'' || c == ' ' || c == ',') {
    c = *++pi;
  }

  start = end = pi;

  while (c != ':' && c != '\"' && c != '\'') {
    if (c != ' ')
      end = pi + 1;
    
    c = *++pi;
  }

  *keysize = end - start;

  *ptr = pi;
  return start;
}

JSON_INLINE
const char*
json_value(const char ** __restrict ptr, size_t * restrict valuesize) {
  const char *pi, *end, *start;
  char        c;

  pi  = *ptr;
  c   = *pi;
  end = pi;

  while (c == ':' || c == '\"' || c == '\"' || c == ' ') {
    c = *++pi;
  }

  start = pi;
  
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

  *valuesize = end - start;

  *ptr = pi;

  return start;
}

json_t*
json_child(json_doc_t  * __restrict doc,
           json_t      * __restrict parent,
           bool                     isvalue) {
  json_t     *root, *child;
  const char *key;
  size_t      keysize;
  char        c;

  if (!doc->ptr || (c = *doc->ptr) == '\0')
    return NULL;

  root = NULL;

  do {
  cont:

    /* skip white spaces */
    do {
      switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case '\0':
          c = *++doc->ptr;
          continue;
        default:
          goto chr;
      }
    } while (true);

  chr:
    /* child */
    switch (c) {
      case '{': {
        json_t *object, *child;
        object = json_calloc(doc, sizeof(json_t));

        if (!parent) {
          root = object;
        } else {
          object->prev  = parent;
          object->next  = parent->child;
          parent->child = object;

          root = object;
        }

        ++doc->ptr;
        if ((child = json_child(doc, object, false))) {
          child->prev   = parent;
          child->next   = object->child;
          object->child = child;
        }

        continue;
      }
      case '[': {
        json_t *object;
        object = json_calloc(doc, sizeof(json_array_t));
        break;
      }
      case '}':
        goto ret;
    }

    /* primitive value */
    if (isvalue) {
      parent->value = json_value(&doc->ptr, &parent->valSize);
      isvalue       = false;

      char jj[1024];
      snprintf(jj, parent->valSize + 1, "%s", parent->value);

      printf("value: %s(%zu)\n", jj, parent->valSize);

      printf("\nparent ch: %p\n", parent);
      return NULL;
    }

    key = json_key(&doc->ptr, &keysize);
    if (key == NULL || *key == '\0')
      break;

    char jj[1024];
    snprintf(jj, keysize + 1, "%s", key);

    printf("key: %s(%zu): ", jj, keysize);

    /* child properties */
    child          = json_calloc(doc, sizeof(json_t));
    child->key     = key;
    child->keySize = keysize;

    printf("\nchild: %p\n", child);

    assert(parent != child);

    if (parent) {
      child->prev   = parent;
      child->next   = parent->child;
      parent->child = child;
    }

    c = *doc->ptr;

    /* jump to value */
    do {
      switch (c) {
        case ' ':
        case '"':
        case '\'':
        case '\t':
        case '\n':
        case '\r':
        case ':':
          c = *++doc->ptr;
          continue;
        default:
          goto val;
      }
    } while (true);

  val:
    child->child = json_child(doc, child, true);
  } while ((c = *doc->ptr) != '\0' && (c = *++doc->ptr) != '\0');

ret:
  return root;
}

json_t*
json_parse(const char * __restrict contents) {
  json_doc_t *doc;
  json_t     *object;
  const char *ptr;

  doc          = calloc(1, sizeof(*doc));
  doc->memroot = calloc(1, JSON_MEM_PAGE);
  ptr          = contents;
  doc->ptr = ptr;
  object       = json_calloc(doc, sizeof(json_t));
  object       = json_child(doc, NULL, false);

  return object;
}

json_t*
json_get(json_t * __restrict object, const char * __restrict key) {
  json_t *iter;
  
  if (!object)
    return NULL;
    
  iter = object->child;
  if (!(iter = object->child))
    return NULL;
  
  while (strcmp(iter->key, key) == 0)
    iter = iter->next;
  
  return iter;
}

void
json_set(json_t * __restrict object, const char * __restrict key) {
  
}
