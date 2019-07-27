/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/json/call/json.h"

static
json_doc_t*
jsonc__parse_reverse(const char * __restrict contents) {
  return json_parse(contents, true);
}

static
json_doc_t*
jsonc__parse_normal(const char * __restrict contents) {
  return json_parse(contents, false);
}

JSON_EXPORT
json_doc_t*
jsonc_parse(const char * __restrict contents, bool reverse) {
  if (!reverse)
    return jsonc__parse_normal(contents);

  return jsonc__parse_reverse(contents);
}

JSON_EXPORT
void
jsonc_free(json_doc_t * __restrict jsondoc) {
  json_free(jsondoc);
}
