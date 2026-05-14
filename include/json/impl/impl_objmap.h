/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_impl_objmap_h
#define json_impl_objmap_h

#include "../json.h"
#include "impl_mem.h"
#include "../util.h"

JSON_INLINE
void
json_objmap(json_t        * __restrict obj,
            json_objmap_t * __restrict objmap,
            size_t                     count) {
  json_objmap_t *item;
  size_t         start, end, i;
  int            keysize;
  char           first;

  if (!obj || obj->type != JSON_OBJECT || !(obj = obj->value))
    return;

  start = 0;
  end   = count;

  while (obj) {
    keysize = obj->keysize;
    first   = obj->key ? obj->key[0] : '\0';
    for (i = start; i < end; i++) {
      item = &objmap[i];

      if (!item->keysize) {
        item->keysize = strlen(item->key);
        if (item->keysize <= 8)
          item->keypack = json__pack8(item->key, item->keysize);
      }

      if ((size_t)keysize == item->keysize
          && first == item->key[0]
          && (item->keysize <= 8
              ? json__bytes_eq_packed(obj->key,
                                      item->keysize,
                                      item->keypack,
                                      item->keysize)
              : memcmp(item->key, obj->key, item->keysize) == 0)) {
        item->object = obj;
        if (i == start)
          start++;
        else if (i == end - 1)
          end--;

        /* duplicated keys are not allowed for now */
        break;
      }
    }
    obj = obj->next;
  }
}

JSON_INLINE
void
json_objmap_call(json_t        * __restrict obj,
                 json_objmap_t * __restrict objmap,
                 size_t                     count,
                 bool          * __restrict stop) {
  json_objmap_t *item;
  size_t         i;

  if (!obj || obj->type != JSON_OBJECT)
    return;

  json_objmap(obj, objmap, count);

  for (i = 0; i < count; i++) {
    if (stop && *stop)
      break;

    item = &objmap[i];
    if (item->object) {
      if (item->foundFunc.func)
        item->foundFunc.func(item->object, item->foundFunc.param);
    } else if (item->notFoundFunc.func) {
      item->notFoundFunc.func(item->object, item->notFoundFunc.param);
    }
  }
}

#endif /* json_impl_objmap_h */
