/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_objmap_h
#define json_objmap_h

#include "common.h"

typedef void (*json_objmap_fun_t)(json_t * __restrict object,
                                  void * __restrict userdata);

typedef struct json_func_t {
  json_objmap_fun_t func;
  void             *param;
} json_func_t;

typedef struct json_objmap_t {
  const char       *key;
  json_t           *object; /* found json object */
  void             *userdata;
  json_func_t       foundFunc;
  json_func_t       notFoundFunc;
} json_objmap_t;

JSON_INLINE
void
json_objmap(json_t        * __restrict obj,
            json_objmap_t * __restrict objmap,
            size_t                     count);

JSON_INLINE
void
json_objmap_call(json_t        * __restrict obj,
                 json_objmap_t * __restrict objmap,
                 size_t                     count,
                 bool          * __restrict stop);

#include "impl/impl_objmap.h"

#endif /* json_objmap_h */
