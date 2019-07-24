.. default-domain:: C

parse json
================================================================================

Header: json/json.h

JSON Document
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
JSON document is returned when parsing json contents is done. This object stores
root JSON object and allocated memories.

It creates pointers to actual data, so you must retain JSON data until you have finished to process json.

You  After you processed the parsed JSON, then you must free this document. 

Table of contents (click to go):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Functions:

1. :c:func:`json_parse`
#. :c:func:`json_free`
#. :c:func:`json_get`
#. :c:func:`json_array`

Functions documentation
~~~~~~~~~~~~~~~~~~~~~~~

.. c:function:: json_doc_t* json_parse(const char * __restrict contents)

    parse json string

    this function parses JSON string and retuns a document which contains:
      - JSON object
      - allocated memory
    after JSON is processed, the object must be freed with json_free()
    
    this library doesn't alloc any memory for JSON itsef and doesn't copy JSON
    contents into a data structure. It only allocs memory for tokens.
    So don't free 'contents' parameter until you finished to process JSON.
    
    Desired order:
     1. Read JSON file
     2. Pass the contents to json_parse()
     3. Process/Handle JSON
     4. free JSON document with json_free()
     5. free `contents`

    Parameters:
      | *[in]*  **contents**  JSON string

    Returns:
      | json document which contains json object as root object

.. c:function:: void json_free(json_doc_t * __restrict jsondoc)

    frees json document and its allocated memory

    Parameters:
      | *[in]* **jsondoc**  JSON document

.. c:function:: const json_t* json_get(const json_t * __restrict object, const char * __restrict key)

    gets value for key

    You should only use this for DEBUG or if you only need to only specific key.
    Desired usage is iterative way:
      You must iterate through json's next and value links.

    Parameters:
      | *[in]* **object**  json object
      | *[in]* **key**     key to find value

    Returns:
      | value found for the key or NULL
      
.. c:function:: const json_array_t* json_array(const json_t * __restrict object) 

    contenient function to cast object's child/value to array

    Parameters:
      | *[in]* **object**  json object

    Returns:
      | json array or NULL
