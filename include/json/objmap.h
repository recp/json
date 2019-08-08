/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_objmap_h
#define json_objmap_h

#include "common.h"

/*
 helpers to create objmap arrays, for example:

  json_objmap_t objmap[] = {
    JSON_OBJMAP_FN("key 1",  func1, param1),
    JSON_OBJMAP_FN("key 2",  func2, param2),
    JSON_OBJMAP_FN("key 3",  func3, param3),
    JSON_OBJMAP_FN("key 4",  func4, param4),
    JSON_OBJMAP_FN("key 5",  func5, param5)
  };

 this may be more readable than manually put values to objmap struct.
 */

#define JSON_OBJMAP_FN(KEY, FUN, PARAM) \
  {\
    .object    = NULL, \
    .key       = KEY,\
    .foundFunc = {FUN, PARAM} \
  }

#define JSON_OBJMAP_OBJ(KEY, USERDATA) \
  {\
    .object    = NULL, \
    .key       = KEY, \
    .userdata  = USERDATA \
  }

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
