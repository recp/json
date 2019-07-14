/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */
#ifndef json_print_h
#define json_print_h

#include "common.h"
#include <stdio.h>
#include <stdlib.h>

JSON_INLINE
void
json_print_pad(json_t *json, int pad) {
  char *key, *value;
  int   i;

  while (json) {
    for (i = 0; i < pad; i++)
      printf("\t");

    if (json->key) {
      key = alloca(json->keySize);
      snprintf(key, json->keySize + 1, "%s", json->key);
      printf("\"%s\": ", key);
    }
    switch (json->type) {
      case JSON_OBJECT:
        printf("{\n");
        json_print_pad(json->child, pad + 1);

        for (i = 0; i < pad; i++)
          printf("\t");
        printf("}");

        if (json->next)
          printf(",");

        printf("\n");
        break;

      case JSON_STRING:
        value = alloca(json->valSize);
        snprintf(value, json->valSize + 1, "%s", json->value);

        printf("\"%s\"", value);

        if (json->next)
          printf(",");
        printf("\n");
        break;
      default:
        break;
    }



    json = json->next;
  }
}

JSON_INLINE
void
json_print(json_t *json) {
  printf("json ( %p ):\n", json);
  json_print_pad(json, 0);
  printf("\n");
}

#endif /* json_print_h */
