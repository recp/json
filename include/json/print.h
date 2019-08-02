/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_print_h
#define json_print_h

#include "common.h"
#include "json.h"
#include "util.h"
#include <stdio.h>

/*!
 * @brief print json with option
 *
 * @param[in] ostream stream to print
 * @param[in] json    json object
 * @param[in] opt     0: uglify, 1: pretty, 2: human
 */
JSON_INLINE
void
json_print_ex(const FILE   * __restrict ostream,
              const json_t * __restrict json,
              int                       opt) {
  const json_t *parent;
  int           pad, i;

  pad    = 0;
  parent = NULL;

  while (json) {
    if (opt > 0) {
      for (i = 0; i < pad; i++)
        printf("\t");

      if (json->key)
        printf("\"%.*s\": ", json->keySize, json->key);
    } else {
      if (json->key)
        printf("\"%.*s\":", json->keySize, json->key);
    }

    switch (json->type) {
      case JSON_OBJECT:
        if (json->value) {
          if (opt > 0)
            printf("{\n");
          else
            printf("{");

          pad++;

          /* parent = json; */
          json = json->value;
          continue;
        }
        break;

      case JSON_STRING:
        printf("\"%.*s\"", json->valSize, json_string(json));

        if (json->next)
          printf(",");

        if (opt > 0)
          printf("\n");
        break;

      case JSON_ARRAY:
        if (json->value) {
          if (opt > 0) {
            if (opt > 1)
              printf("(%d) [\n", ((json_array_t *)json)->count);
            else
              printf("[\n");
          } else {
            printf("[");
          }

          pad++;

          /* parent = json; */
          json = json->value;
          continue;
        }
        break;
      default:
        break;
    }

    if (json->next) {
      json = json->next;
    } else if ((parent = json->parent)) {
      do {
        --pad;

        if (opt > 0) {
          for (i = 0; i < pad; i++)
            printf("\t");
        }

        if (parent->type == JSON_OBJECT)
          printf("}");
        else if (parent->type == JSON_ARRAY)
          printf("]");

        if (parent->next)
          printf(",");

        if (opt > 0)
          printf("\n");

        json   = parent->next;
        parent = parent->parent;
      } while (!json && parent);
    } else {
      break;
    }
  } /* while (json)  */
}

/*!
 * @brief print json
 *
 * @param[in] ostream stream to print
 * @param[in] json    json object
 */
JSON_INLINE
void
json_print_pretty(const FILE   * __restrict ostream,
                  const json_t * __restrict json) {
  json_print_ex(ostream, json, 1);
}

/*!
 * @brief print json for humans
 *
 * @param[in] ostream stream to print
 * @param[in] json    json object
 */
JSON_INLINE
void
json_print_human(const FILE   * __restrict ostream,
                 const json_t * __restrict json) {
  printf("json ( %p ):\n", json);
  json_print_ex(ostream, json, 2);
  printf("\n");
}

/*!
 * @brief print json
 *
 * @param[in] ostream stream to print
 * @param[in] json    json object
 */
JSON_INLINE
void
json_print_uglify(const FILE   * __restrict ostream,
                  const json_t * __restrict json) {
  json_print_ex(ostream, json, 0);
}

#endif /* json_print_h */
