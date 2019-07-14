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
  json_t     *obj;
  const char *key;
  int         keysize;
  char        c;
  bool        lookingForKey;

  if (!doc->ptr || (c = *doc->ptr) == '\0')
    return;

  key            = NULL;
  obj            = NULL;
  keysize        = 0;
  lookingForKey  = false;

  do {
  again:
    /* child */
    switch (c) {
<<<<<<< Updated upstream
      case '{': {
        json_t *object, *child;
        object       = json_calloc(doc, sizeof(json_t));
        
        root = object;

        if (parent) {
          object->prev  = parent;
          object->next  = parent->child;
          parent->child = object;
        }  else {
          object->type = JSON_OBJECT;
         }
=======
      /* trim */
      case ' ':
      case '\r':
      case '\n':
      case '\t':
        c = *++doc->ptr;
        continue;
      case '{':
      case '[': {
        obj = json_calloc(doc, sizeof(json_t));

        if (c == '{') {
          obj->type     = JSON_OBJECT;
          lookingForKey = true;
        } else {
          obj->type = JSON_ARRAY;
        }
>>>>>>> Stashed changes

        /* parent must not be NULL */
        obj->prev     = parent;
        obj->next     = parent->child;
        parent->child = obj;

        if (key) {
          obj->key     = key;
          obj->keySize = keysize;
          key          = NULL;
        }

        c = *++doc->ptr;
        break;
      }
      case '}':
<<<<<<< Updated upstream
        goto ret;
    }

    /* primitive value */
    if (isvalue) {
      parent->value = json_value(&doc->ptr, &parent->valSize);
      isvalue       = false;
      parent->type  = JSON_STRING;
      return NULL;
    }

    key = json_key(&doc->ptr, &keysize);
    if (key == NULL || *key == '\0')
      break;

    /* child properties */
    child          = json_calloc(doc, sizeof(json_t));
    child->key     = key;
    child->keySize = keysize;
    child->type    = JSON_OBJECT;

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
=======
      case ']':
        c   = *++doc->ptr;
        obj = NULL;
        break;
      case ',': {
        lookingForKey = true;
>>>>>>> Stashed changes
      }
      default: {
        /* looking for key */
        if (lookingForKey) {
          key = json_key(&doc->ptr, &keysize);
          if (key == NULL || ((c = *key) == '\0'))
            goto err;
          lookingForKey = false;
          
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

<<<<<<< Updated upstream
  val:
    if ((child->child = json_child(doc, child, true))) {
      // if (child->type == JSON_UNKOWN)
        // child->type = JSON_OBJECT;
=======
        /* looking for value */
        else {
          printf("looking for value\n");
          json_t *val;
          val       = json_calloc(doc, sizeof(json_t));
          val->type = JSON_STRING;

          /* parent must not be NULL */
          val->prev  = obj;
          val->next  = obj->child;
          obj->child = val;

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
>>>>>>> Stashed changes
    }
  } while ((c = *doc->ptr) != '\0' && (c = *++doc->ptr) != '\0');

err:
ret:
  return ;
}

json_t*
json_parse(const char * __restrict contents) {
  json_doc_t *doc;
  json_t      tmproot;

  doc           = calloc(1, sizeof(*doc));
  doc->memroot  = calloc(1, JSON_MEM_PAGE);
  doc->ptr      = contents;
  tmproot.child = NULL;

  json_child(doc, &tmproot);

  if (tmproot.child) {
    tmproot.child->prev = NULL;
  }

  return tmproot.child;
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
