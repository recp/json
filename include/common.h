/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_common_h
#define json_common_h

#define _USE_MATH_DEFINES /* for windows */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if defined(_MSC_VER)
#  ifdef JSON_DLL
#    define JSON_EXPORT __declspec(dllexport)
#  else
#    define JSON_EXPORT __declspec(dllimport)
#  endif
#  define JSON_INLINE __forceinline
#else
#  define JSON_EXPORT __attribute__((visibility("default")))
#  define JSON_INLINE static inline __attribute((always_inline))
#endif

#endif /* json_common_h */
