.. default-domain:: C

utils / helpers
================================================================================

Header: json/util.h

Inline helpers to make things easier while process JSON. 
Most of uitl functions expects default value, so if it fails to convert string to a number or boolean then that default value will be returned.

Table of contents (click to go):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Functions:

1. :c:func:`json_int32`
#. :c:func:`json_uint32`
#. :c:func:`json_int64`
#. :c:func:`json_uint64`
#. :c:func:`json_float`
#. :c:func:`json_double`
#. :c:func:`json_bool`
#. :c:func:`json_string`
#. :c:func:`json_string_dup`
#. :c:func:`json_key_eq`

Functions documentation
~~~~~~~~~~~~~~~~~~~~~~~

.. c:function:: int32_t json_int32(const json_t * __restrict object, int32_t defaultValue) 

    | creates number (int32) from string value

    Parameters:
      | *[in]*  **object**         json object
      | *[in]*  **defaultValue**   default value if operation fails

    Returns:
      number

.. c:function:: uint32_t json_uint32(const json_t * __restrict object, uint32_t defaultValue) 

    | creates number (uint32) from string value

    Parameters:
      | *[in]*  **object**         json object
      | *[in]*  **defaultValue**   default value if operation fails

    Returns:
      number

.. c:function:: int64_t json_int64(const json_t * __restrict object, int64_t defaultValue) 

    | creates number (int64) from string value

    Parameters:
      | *[in]*  **object**         json object
      | *[in]*  **defaultValue**   default value if operation fails

    Returns:
      number

.. c:function:: int64_t json_uint64(const json_t * __restrict object, uint64_t defaultValue) 

    | creates number (uint64) from string value

    Parameters:
      | *[in]*  **object**         json object
      | *[in]*  **defaultValue**   default value if operation fails

    Returns:
      number

.. c:function:: float json_float(const json_t * __restrict object, float defaultValue) 

    | creates number (float) from string value

    Parameters:
      | *[in]*  **object**         json object
      | *[in]*  **defaultValue**   default value if operation fails

    Returns:
      number

.. c:function:: double json_double(const json_t * __restrict object, double defaultValue) 

    | creates number (double) from string value

    Parameters:
      | *[in]*  **object**         json object
      | *[in]*  **defaultValue**   default value if operation fails

    Returns:
      number

.. c:function:: int json_bool(const json_t * __restrict object, int defaultValue) 

    | creates boolean from string value

    | it returns integer to separate default value from true or false

    Parameters:
      | *[in]*  **object**         json object
      | *[in]*  **defaultValue**   default value if operation fails

    Returns:
      boolean values as integer: 1 true, 0 false, error: defaultValue

.. c:function:: const char* json_string(const json_t * __restrict object) 

    | return non-NULL terminated string value
    | you must use object->valSize to copy, compare ... string

    Parameters:
      | *[in]*  **object** json object

    Returns:
      non-NULL terminated string value (pointer only)

.. c:function:: char* json_string_dup(const json_t * __restrict object) 

    | return non-NULL terminated string value
    | you must use object->valSize to copy, compare ... string

    Parameters:
      | *[in]*  **object** json object

    Returns:
      NULL terminated duplicated string value

.. c:function:: bool json_key_eq(const json_t * __restrict obj, const char * __restrict str)

    | compares json key with a string|

    Parameters:
      | *[in]*  **obj**  json object
      | *[in]*  **str**  string to compare

    Returns:
      true if str is equals to json's key
