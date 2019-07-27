/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/json/call/json.h"

JSON_EXPORT
json_doc_t*
jsonc_parse(const char * __restrict contents) {
  return json_parse(contents);
}

JSON_EXPORT
void
jsonc_free(json_doc_t * __restrict jsondoc) {
  json_free(jsondoc);
}
