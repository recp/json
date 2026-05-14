/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_util_h
#define json_util_h

#include "common.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

JSON_INLINE
const char*
json__num_begin(const json_t * __restrict object,
                const char ** __restrict end) {
  const char *p;

  if (!object || object->type != JSON_STRING || !object->value) {
    *end = NULL;
    return NULL;
  }

  p = object->value;
  *end = p + object->valsize;
  return p;
}

JSON_INLINE
uint64_t
json__parse_uint64(const char * __restrict p,
                   const char * __restrict end,
                   uint64_t                 defaultValue) {
  uint64_t value;

  if (!p || p >= end)
    return defaultValue;

  value = 0;
  if (*p == '+')
    p++;
  if (p >= end || *p < '0' || *p > '9')
    return defaultValue;

  do {
    value = value * 10u + (uint64_t)(*p - '0');
    p++;
  } while (p < end && *p >= '0' && *p <= '9');

  return value;
}

JSON_INLINE
uint32_t
json__parse_uint32(const char * __restrict p,
                   const char * __restrict end,
                   uint32_t                 defaultValue) {
  uint32_t value;

  if (!p || p >= end)
    return defaultValue;

  value = 0;
  if (*p == '+')
    p++;
  if (p >= end || *p < '0' || *p > '9')
    return defaultValue;

  do {
    value = value * 10u + (uint32_t)(*p - '0');
    p++;
  } while (p < end && *p >= '0' && *p <= '9');

  return value;
}

JSON_INLINE
int64_t
json__parse_int64(const char * __restrict p,
                  const char * __restrict end,
                  int64_t                  defaultValue) {
  uint64_t value;
  bool     neg;

  if (!p || p >= end)
    return defaultValue;

  neg = *p == '-';
  if (neg || *p == '+')
    p++;

  value = json__parse_uint64(p, end, UINT64_MAX);
  if (value == UINT64_MAX)
    return defaultValue;

  return neg ? -(int64_t)value : (int64_t)value;
}

JSON_INLINE
int32_t
json__parse_int32(const char * __restrict p,
                  const char * __restrict end,
                  int32_t                  defaultValue) {
  uint32_t value;
  bool     neg, hasDigits;

  if (!p || p >= end)
    return defaultValue;

  neg = *p == '-';
  if (neg || *p == '+')
    p++;

  value     = 0;
  hasDigits = false;
  while (p < end && *p >= '0' && *p <= '9') {
    value = value * 10u + (uint32_t)(*p - '0');
    p++;
    hasDigits = true;
  }

  if (!hasDigits)
    return defaultValue;

  return neg ? -(int32_t)value : (int32_t)value;
}

JSON_INLINE
double
json__pow10i(int exp) {
  static const double pow10[] = {
    1.0e1, 1.0e2, 1.0e4, 1.0e8, 1.0e16,
    1.0e32, 1.0e64, 1.0e128, 1.0e256
  };
  double       scale;
  unsigned int e, i;
  bool         neg;

  if (exp == 0)
    return 1.0;

  neg   = exp < 0;
  e     = neg ? (unsigned int)-exp : (unsigned int)exp;
  scale = 1.0;
  for (i = 0; e && i < JSON_ARR_LEN(pow10); i++, e >>= 1) {
    if (e & 1u)
      scale *= pow10[i];
  }

  return neg ? 1.0 / scale : scale;
}

JSON_INLINE
double
json__parse_double(const char * __restrict p,
                   const char * __restrict end,
                   double                   defaultValue) {
  double value, place;
  int    exp, expNeg;
  bool   neg, hasDigits;

  if (!p || p >= end)
    return defaultValue;

  neg = *p == '-';
  if (neg || *p == '+')
    p++;

  value     = 0.0;
  hasDigits = false;
  while (p < end && *p >= '0' && *p <= '9') {
    value = value * 10.0 + (double)(*p - '0');
    p++;
    hasDigits = true;
  }

  if (p < end && *p == '.') {
    p++;
    place = 0.1;
    while (p < end && *p >= '0' && *p <= '9') {
      value += (double)(*p - '0') * place;
      place *= 0.1;
      p++;
      hasDigits = true;
    }
  }

  if (!hasDigits)
    return defaultValue;

  if (p < end && (*p == 'e' || *p == 'E')) {
    p++;
    expNeg = p < end && *p == '-';
    if (expNeg || (p < end && *p == '+'))
      p++;

    exp = 0;
    while (p < end && *p >= '0' && *p <= '9') {
      exp = exp * 10 + (*p - '0');
      p++;
    }
    if (exp)
      value *= json__pow10i(expNeg ? -exp : exp);
  }

  return neg ? -value : value;
}

JSON_INLINE
float
json__pow10if(int exp) {
  static const float pow10[] = {
    1.0e1f, 1.0e2f, 1.0e4f, 1.0e8f, 1.0e16f, 1.0e32f
  };
  float        scale;
  unsigned int e, i;
  bool         neg;

  if (exp == 0)
    return 1.0f;

  neg   = exp < 0;
  e     = neg ? (unsigned int)-exp : (unsigned int)exp;
  scale = 1.0f;
  for (i = 0; e && i < JSON_ARR_LEN(pow10); i++, e >>= 1) {
    if (e & 1u)
      scale *= pow10[i];
  }

  return neg ? 1.0f / scale : scale;
}

JSON_INLINE
float
json__parse_float(const char * __restrict p,
                  const char * __restrict end,
                  float                    defaultValue) {
  float value, place;
  int   exp, expNeg;
  bool  neg, hasDigits;

  if (!p || p >= end)
    return defaultValue;

  neg = *p == '-';
  if (neg || *p == '+')
    p++;

  value     = 0.0f;
  hasDigits = false;
  while (p < end && *p >= '0' && *p <= '9') {
    value = value * 10.0f + (float)(*p - '0');
    p++;
    hasDigits = true;
  }

  if (p < end && *p == '.') {
    p++;
    place = 0.1f;
    while (p < end && *p >= '0' && *p <= '9') {
      value += (float)(*p - '0') * place;
      place *= 0.1f;
      p++;
      hasDigits = true;
    }
  }

  if (!hasDigits)
    return defaultValue;

  if (p < end && (*p == 'e' || *p == 'E')) {
    p++;
    expNeg = p < end && *p == '-';
    if (expNeg || (p < end && *p == '+'))
      p++;

    exp = 0;
    while (p < end && *p >= '0' && *p <= '9') {
      exp = exp * 10 + (*p - '0');
      p++;
    }
    if (exp)
      value *= json__pow10if(expNeg ? -exp : exp);
  }

  return neg ? -value : value;
}

/*!
 * @brief creates number (int32) from string value
 *
 * @param[in] object       json object
 * @param[in] defaultValue default value if operation fails
 * @return number
 */
JSON_INLINE
int32_t
json_int32(const json_t * __restrict object, int32_t defaultValue) {
  const char *end;

  return json__parse_int32(json__num_begin(object, &end), end, defaultValue);
}

/*!
 * @brief creates number (uint32) from string value
 *
 * @param[in] object       json object
 * @param[in] defaultValue default value if operation fails
 * @return number
 */
JSON_INLINE
uint32_t
json_uint32(const json_t * __restrict object, uint32_t defaultValue) {
  const char *end;

  return json__parse_uint32(json__num_begin(object, &end), end, defaultValue);
}

/*!
 * @brief creates number (int64) from string value
 *
 * @param[in] object       json object
 * @param[in] defaultValue default value if operation fails
 * @return number
 */
JSON_INLINE
int64_t
json_int64(const json_t * __restrict object, int64_t defaultValue) {
  const char *end;

  return json__parse_int64(json__num_begin(object, &end), end, defaultValue);
}

/*!
 * @brief creates number (uint64) from string value
 *
 * @param[in] object       json object
 * @param[in] defaultValue default value if operation fails
 * @return number
 */
JSON_INLINE
uint64_t
json_uint64(const json_t * __restrict object, uint64_t defaultValue) {
  const char *end;

  return json__parse_uint64(json__num_begin(object, &end), end, defaultValue);
}

/*!
 * @brief creates number (float) from string value
 *
 * @param[in] object       json object
 * @param[in] defaultValue default value if operation fails
 * @return number
 */
JSON_INLINE
float
json_float(const json_t * __restrict object, float defaultValue) {
  const char *end;

  return json__parse_float(json__num_begin(object, &end), end, defaultValue);
}

/*!
 * @brief creates number (double) from string value
 *
 * @param[in] object       json object
 * @param[in] defaultValue default value if operation fails
 * @return number
 */
JSON_INLINE
double
json_double(const json_t * __restrict object, double defaultValue) {
  const char *end;

  return json__parse_double(json__num_begin(object, &end), end, defaultValue);
}

/*!
 * @brief creates boolean from string value
 *
 * @param[in] object       json object
 * @param[in] defaultValue default value if operation fails
 * @return boolean values as integer: 1 true, 0 false, error: defaultValue
 */
JSON_INLINE
int
json_bool(const json_t * __restrict object, int defaultValue) {
  const char *boolString;
  char        first;

  if (!object || object->type != JSON_STRING || !(boolString = object->value))
    return defaultValue;

  first = boolString[0];

  if (first == 't' || first == '1') {
    return 1;
  } else if (first == 'n' || first == '0') {
    return 0;
  }

  return defaultValue;
}

/*!
 * @brief return non-NULL terminated string value
 *
 * you must use object->valSize to copy, compare ... string
 *
 * @param[in] object json object
 * @return non-NULL terminated string value (pointer only)
 */
JSON_INLINE
const char*
json_string(const json_t * __restrict object) {
  return (const char *)object->value;
}

JSON_INLINE
uint32_t
json__load4(const char * __restrict s) {
  uint32_t out;

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  memcpy(&out, s, sizeof(out));
  return out;
#elif defined(_WIN32) || defined(_M_IX86) || defined(_M_X64)                 \
      || defined(_M_ARM64) || defined(__i386__) || defined(__x86_64__)       \
      || defined(__aarch64__)
  memcpy(&out, s, sizeof(out));
  return out;
#else
  return (uint32_t)(uint8_t)s[0]
       | (uint32_t)(uint8_t)s[1] << 8
       | (uint32_t)(uint8_t)s[2] << 16
       | (uint32_t)(uint8_t)s[3] << 24;
#endif
}

JSON_INLINE
uint64_t
json__load8(const char * __restrict s) {
  uint64_t out;

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  memcpy(&out, s, sizeof(out));
  return out;
#elif defined(_WIN32) || defined(_M_IX86) || defined(_M_X64)                 \
      || defined(_M_ARM64) || defined(__i386__) || defined(__x86_64__)       \
      || defined(__aarch64__)
  memcpy(&out, s, sizeof(out));
  return out;
#else
  return (uint64_t)json__load4(s)
       | (uint64_t)json__load4(s + 4) << 32;
#endif
}

JSON_INLINE
uint32_t
json__pack4(const char * __restrict s, size_t len) {
  switch (len) {
    case 0:
      return 0;
    case 1:
      return (uint32_t)(uint8_t)s[0];
    case 2:
      return (uint32_t)(uint8_t)s[0]
           | (uint32_t)(uint8_t)s[1] << 8;
    case 3:
      return (uint32_t)(uint8_t)s[0]
           | (uint32_t)(uint8_t)s[1] << 8
           | (uint32_t)(uint8_t)s[2] << 16;
    default:
      return json__load4(s);
  }
}

JSON_INLINE
uint64_t
json__pack8(const char * __restrict s, size_t len) {
  switch (len) {
    case 0:
      return 0;
    case 1:
      return (uint64_t)(uint8_t)s[0];
    case 2:
      return (uint64_t)json__pack4(s, 2);
    case 3:
      return (uint64_t)json__pack4(s, 3);
    case 4:
      return (uint64_t)json__load4(s);
    case 5:
      return (uint64_t)json__load4(s)
           | (uint64_t)(uint8_t)s[4] << 32;
    case 6:
      return (uint64_t)json__load4(s)
           | (uint64_t)(uint8_t)s[4] << 32
           | (uint64_t)(uint8_t)s[5] << 40;
    case 7:
      return (uint64_t)json__load4(s)
           | (uint64_t)(uint8_t)s[4] << 32
           | (uint64_t)(uint8_t)s[5] << 40
           | (uint64_t)(uint8_t)s[6] << 48;
    default:
      return json__load8(s);
  }
}

JSON_INLINE
bool
json__bytes_eq(const char * __restrict a,
               const char * __restrict b,
               size_t                   len) {
  return len == 0 || (a[0] == b[0] && memcmp(a, b, len) == 0);
}

JSON_INLINE
bool
json__bytes_eq_packed4(const char * __restrict a,
                       size_t                   len,
                       uint32_t                 packed,
                       size_t                   expectedLen) {
  if (len != expectedLen || expectedLen > 4)
    return false;

  if (expectedLen > 0 && (uint8_t)a[0] != (uint8_t)packed)
    return false;

  return expectedLen == 4
         ? json__load4(a) == packed
         : json__pack4(a, len) == packed;
}

JSON_INLINE
bool
json__bytes_eq_packed8(const char * __restrict a,
                       size_t                   len,
                       uint64_t                 packed,
                       size_t                   expectedLen) {
  if (len != expectedLen || expectedLen < 5 || expectedLen > 8)
    return false;

  if ((uint8_t)a[0] != (uint8_t)packed)
    return false;

  return expectedLen == 8
         ? json__load8(a) == packed
         : json__pack8(a, len) == packed;
}

JSON_INLINE
bool
json__bytes_eq_packed(const char * __restrict a,
                      size_t                   len,
                      uint64_t                 packed,
                      size_t                   expectedLen) {
  return expectedLen <= 4
         ? json__bytes_eq_packed4(a, len, (uint32_t)packed, expectedLen)
         : json__bytes_eq_packed8(a, len, packed, expectedLen);
}

/*!
 * @brief return json object from value
 */
JSON_INLINE
json_t*
json_json(const json_t * __restrict object) {
  return (json_t *)object->value;
}

/*!
 * @brief return NULL terminated duplicated string value
 *
 * value must be free-ed when no longer needed
 *
 * @param[in] object json object
 * @return NULL terminated duplicated string value
 */
JSON_INLINE
char*
json_string_dup(const json_t * __restrict object) {
  char *s;

  s = malloc(object->valsize + 1);
  memcpy(s, object->value, object->valsize);
  s[object->valsize] = '\0';

  return s;
}

/*!
 * @brief compares json key with a string
 *
 * @param[in] obj json object
 * @param[in] str string to compare (must be NULL terminated)
 * @return true if str is equals to json's key
 */
JSON_INLINE
bool
json_key_eq(const json_t * __restrict obj, const char * __restrict str) {
  size_t strsize;

  if (!obj || !str || !obj->key)
    return false;
    
  if ((strsize = strlen(str)) != (size_t)obj->keysize)
    return false;

  return json__bytes_eq(str, obj->key, strsize);
}

/*!
 * @brief compares json key with a string
 *
 * @param[in] obj     json object
 * @param[in] str     string to compare
 * @param[in] strsize size of string to compare
 * @return true if str is equals to json's key
 */
JSON_INLINE
bool
json_key_eqsz(const json_t * __restrict obj,
              const char   * __restrict str,
              size_t                    strsize) {
  if (!obj || !str || !obj->key)
    return false;

  if (strsize != (size_t)obj->keysize)
    return false;

  return json__bytes_eq(str, obj->key, strsize);
}

JSON_INLINE
bool
json_key_eq_packed4(const json_t * __restrict obj,
                    uint32_t                  packed,
                    size_t                    strsize) {
  if (!obj || !obj->key)
    return false;

  return json__bytes_eq_packed4(obj->key,
                                (size_t)obj->keysize,
                                packed,
                                strsize);
}

JSON_INLINE
bool
json_key_eq_packed8(const json_t * __restrict obj,
                    uint64_t                  packed,
                    size_t                    strsize) {
  if (!obj || !obj->key)
    return false;

  return json__bytes_eq_packed8(obj->key,
                                (size_t)obj->keysize,
                                packed,
                                strsize);
}

JSON_INLINE
bool
json_key_eq_packed(const json_t * __restrict obj,
                   uint64_t                  packed,
                   size_t                    strsize) {
  if (!obj || !obj->key)
    return false;

  return json__bytes_eq_packed(obj->key, (size_t)obj->keysize, packed, strsize);
}

/*!
 * @brief compares json value with a string
 *
 * @param[in] obj json object
 * @param[in] str string to compare (must be NULL terminated)
 * @return true if str is equals to json's value
 */
JSON_INLINE
bool
json_val_eq(const json_t * __restrict obj, const char * __restrict str) {
  size_t strsize;

  if (!obj || !str || !obj->value)
    return false;
    
  if ((strsize = strlen(str)) != (size_t)obj->valsize)
    return false;

  return json__bytes_eq(str, obj->value, strsize);
}

/*!
 * @brief compares json value with a string
 *
 * @param[in] obj     json object
 * @param[in] str     string to compare
 * @param[in] strsize size of string to compare
 * @return true if str is equals to json's value
 */
JSON_INLINE
bool
json_val_eqsz(const json_t * __restrict obj,
              const char   * __restrict str,
              size_t                    strsize) {
  if (!obj || !str || !obj->value)
    return false;

  if (strsize != (size_t)obj->valsize)
    return false;

  return json__bytes_eq(str, obj->value, strsize);
}

JSON_INLINE
bool
json_val_eq_packed4(const json_t * __restrict obj,
                    uint32_t                  packed,
                    size_t                    strsize) {
  if (!obj || !obj->value)
    return false;

  return json__bytes_eq_packed4(obj->value,
                                (size_t)obj->valsize,
                                packed,
                                strsize);
}

JSON_INLINE
bool
json_val_eq_packed8(const json_t * __restrict obj,
                    uint64_t                  packed,
                    size_t                    strsize) {
  if (!obj || !obj->value)
    return false;

  return json__bytes_eq_packed8(obj->value,
                                (size_t)obj->valsize,
                                packed,
                                strsize);
}

JSON_INLINE
bool
json_val_eq_packed(const json_t * __restrict obj,
                   uint64_t                  packed,
                   size_t                    strsize) {
  if (!obj || !obj->value)
    return false;

  return json__bytes_eq_packed(obj->value,
                               (size_t)obj->valsize,
                               packed,
                               strsize);
}

/*!
 * @brief returns parent object if available
 *
 * @param[in] obj json object
 * @return returns parent object if available
 */
JSON_INLINE
json_t*
json_parent(const json_t * __restrict obj) {
  /*
  const json_t *prev;
  if (!obj)
    return NULL;

  do {
    prev = obj;
    obj  = obj->prev;
  } while (obj && obj->value != prev);

  return obj;*/

  return obj->parent;
}

#endif /* json_util_h */
