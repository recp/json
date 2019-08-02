/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_call_json_h
#define json_call_json_h
#ifdef __cplusplus
extern "C" {
#endif

#include "../common.h"
#include "../json.h"

/*!
 * @brief parse json string
 *
 * this function parses JSON string and retuns a document which contains:
 *   - JSON object
 *   - allocated memory
 * after JSON is processed, the object must be freed with json_free()
 *
 * this library doesn't alloc any memory for JSON itsef and doesn't copy JSON
 * contents into a data structure. It only allocs memory for tokens.
 * So don't free 'contents' parameter until you finished to process JSON.
 *
 * Desired order:
 *  1. Read JSON file
 *  2. Pass the contents to json_parse()
 *  3. Process/Handle JSON
 *  4. free JSON document with json_free()
 *  5. free `contents`
 *
 * @param[in] contents JSON string
 * @param[in] reverse  store members, array items as reverse order
 * @return json document which contains json object as root object
 */
JSON_EXPORT
json_doc_t*
jsonc_parse(const char * __restrict contents, bool reverse);

/*!
 * @brief frees json document and its allocated memory
 */
JSON_EXPORT
void
jsonc_free(json_doc_t * __restrict jsondoc);

#ifdef __cplusplus
}
#endif
#endif /* json_call_json_h */
