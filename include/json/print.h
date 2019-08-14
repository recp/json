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
json_print_ex(FILE   * __restrict ostream,
              json_t * __restrict json,
              int                 opt) {
  const json_t *parent;
  int           pad, i;

  pad    = 0;
  parent = NULL;

  while (json) {
    if (opt > 0) {
      for (i = 0; i < pad; i++)
        fprintf(ostream, "\t");

      if (json->key)
        fprintf(ostream, "\"%.*s\": ", json->keySize, json->key);
    } else {
      if (json->key)
        fprintf(ostream, "\"%.*s\":", json->keySize, json->key);
    }

    switch (json->type) {
      case JSON_OBJECT:
        if (json->value) {
          if (opt > 0)
            fprintf(ostream, "{\n");
          else
            fprintf(ostream, "{");

          pad++;

          /* parent = json; */
          json = json->value;
          continue;
        }
        break;

      case JSON_STRING:
        fprintf(ostream, "\"%.*s\"", json->valSize, json_string(json));

        if (json->next)
          fprintf(ostream, ",");

        if (opt > 0)
          fprintf(ostream, "\n");
        break;

      case JSON_ARRAY:
        if (json->value) {
          if (opt > 0) {
            if (opt > 1)
              fprintf(ostream, "(%d) [\n", ((json_array_t *)json)->count);
            else
              fprintf(ostream, "[\n");
          } else {
            fprintf(ostream, "[");
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
            fprintf(ostream, "\t");
        }

        if (parent->type == JSON_OBJECT)
          fprintf(ostream, "}");
        else if (parent->type == JSON_ARRAY)
          fprintf(ostream, "]");

        if (parent->next)
          fprintf(ostream, ",");

        if (opt > 0)
          fprintf(ostream, "\n");

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
json_print_pretty(FILE   * __restrict ostream,
                  json_t * __restrict json) {
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
json_print_human(FILE   * __restrict ostream,
                 json_t * __restrict json) {
  printf("json ( %p ):\n", (void *)json);
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
json_print_uglify(FILE   * __restrict ostream,
                  json_t * __restrict json) {
  json_print_ex(ostream, json, 0);
}

#endif /* json_print_h */
