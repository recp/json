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

void
print_json_pad(json_t *json, int pad) {
  char *key, *value;
  int   i;

  while (json) {
    for (i = 0; i < pad; i++)
      printf("\t");
    switch (json->type) {
      case JSON_OBJECT:
        printf("{\n");
        print_json_pad(json->child, pad + 1);

        for (i = 0; i < pad; i++)
          printf("\t");
        printf("}");

        if (json->next)
          printf(",");

        printf("\n");
        break;

      case JSON_STRING:
        key   = alloca(json->keySize);
        value = alloca(json->valSize);

        snprintf(key,   json->keySize + 1, "%s", json->key);
        snprintf(value, json->valSize + 1, "%s", json->value);

        printf("\"%s\": \"%s\"", key, value);

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

void
print_json(json_t *json) {
  printf("json ( %p ):\n", json);
  print_json_pad(json, 0);
  printf("\n");
}

#endif /* json_print_h */
