/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../json.h"
#include "impl_mem.h"

JSON_INLINE
const void*
json__impl_key(const char ** __restrict ptr, int * __restrict keysize) {
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
void*
json__impl_value(const char ** __restrict ptr, int * __restrict valuesize) {
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

  return (void *)start;
}

JSON_INLINE
json_doc_t*
json_parse(const char * __restrict contents, bool reverse) {
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

  memset(&tmproot, 0, sizeof(tmproot));
  tmproot.type   = JSON_OBJECT;

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
          obj           = json__impl_calloc(doc, sizeof(json_t));
          obj->type     = JSON_OBJECT;
          lookingForKey = true;
        } else {
          obj       = json__impl_calloc(doc, sizeof(json_array_t));
          obj->type = JSON_ARRAY;
        }

        /* parent must not be NULL */

        if (!reverse) {
          if (!parent->next) {
            parent->next = obj;
            obj->prev    = parent;
          } else {
            json_json(parent)->next = obj;
            obj->prev               = parent->value;
          }
        } else {
          if (parent->value)
            json_json(parent)->prev = obj;

          obj->prev = parent;
          obj->next = parent->value;
        }

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
        if (!parent)
          goto err;

        if (!reverse) {
          obj->value = obj->next;
          obj->next  = NULL;
        }

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
          key = json__impl_key(&doc->ptr, &keysize);
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
          val       = json__impl_calloc(doc, sizeof(json_t));
          val->type = JSON_STRING;

          if (obj->type == JSON_ARRAY)
            ++((json_array_t *)obj)->count;

          /* parent must not be NULL */

          if (!reverse) {
            if (!obj->next) {
              obj->next = val;
              val->prev = obj;
            } else {
              json_json(obj)->next = val;
              val->prev            = obj->value;
            }
          } else {
            if (obj->value)
              json_json(obj)->prev = val;

            val->prev  = obj;
            val->next  = obj->value;
          }

          obj->value = val;

          if (key) {
            val->key     = key;
            val->keySize = keysize;
            key          = NULL;
          }

          val->value = json__impl_value(&doc->ptr, &val->valSize);
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

JSON_INLINE
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

JSON_INLINE
const json_t*
json_get(const json_t * __restrict object, const char * __restrict key) {
  const json_t *iter;
  size_t        keysize;
  
  if (!object || !key || !(iter = object->value))
    return NULL;
  
  keysize = strlen(key);
  while (iter && strncmp(iter->key, key, keysize) != 0)
    iter = iter->next;
  
  return iter;
}

JSON_INLINE
const json_array_t*
json_array(const json_t * __restrict object) {
  if (object->type != JSON_ARRAY || !object->value)
    return NULL;
  
  return (void *)object;
}
