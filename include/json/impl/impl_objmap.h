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

  if (!obj || obj->type != JSON_OBJECT || !(obj = obj->value))
    return;

  start = 0;
  end   = count;

  while (obj) {
    for (i = start; i < end; i++) {
      item = &objmap[i];

      if (json_key_eq(obj, item->key)) {
        item->object = obj;
        if (i == start)
          start++;
        else if (i == end - 1)
          end--;
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

  if (obj->type != JSON_OBJECT)
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
