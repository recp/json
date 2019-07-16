/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef json_h
#define json_h

#include "common.h"
#include <stdlib.h>
#include <string.h>

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
 * @return json document which contains json object as root object
 */
JSON_EXPORT
json_doc_t*
json_parse(const char * __restrict contents);

/*!
 * @brief frees json document and its allocated memory
 */
JSON_EXPORT
void
json_free(json_doc_t * __restrict jsondoc);

/*!
 * @brief gets value for key
 *
 * You should only use this for DEBUG or if you only need to only specific key.
 * Desired usage is iterative way:
 *    You must iterate through json's next and value links.
 *
 * @param[in] object json object
 * @param[in] key    key to find value
 * @return value found for the key or NULL
 */
JSON_INLINE
const json_t*
json_get(const json_t * __restrict object, const char * __restrict key) {
  const json_t *iter;
  size_t        keysize;
  
  if (!object || !key || !(iter = object->value))
    return NULL;
  
  keysize = strlen(key);
  while (iter && strncmp(iter->key, key, keysize) != 0)
    iter = iter->next;

  return iter;
}

/*!
 * @brief contenient function to cast object's child/value to array
 *
 * @param[in] object json object
 * @return json array or NULL
 */
JSON_INLINE
const json_array_t*
json_array(json_t * __restrict object) {
  if (object->type != JSON_ARRAY || !object->value)
    return NULL;

  return object->value;
}

#endif /* json_h */
