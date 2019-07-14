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
print_json(json_t *json) {
  char *key, *value;
  
  while (json) {
    printf("\t");
    switch (json->type) {
      case JSON_OBJECT:
        printf("{\n\t");
        print_json(json->child);
        printf("}");
        break;
        
      case JSON_STRING:
        key   = alloca(json->keySize);
        value = alloca(json->valSize);
        
        snprintf(key,   json->keySize + 1, "%s", json->key);
        snprintf(value, json->valSize + 1, "%s", json->value);
        
        printf("\"%s\": \"%s\"\n", key, value);
        break;
      default:
        printf("\n");
        break;
    }
    printf("\t");
    
    json = json->next;
  }
}

#endif /* json_print_h */
