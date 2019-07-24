.. default-domain:: C

print
================================================================================

Header: json/print.h

Print functions

Table of contents (click to go):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Functions:

1. :c:func:`json_print`
#. :c:func:`json_print_pad`

Functions documentation
~~~~~~~~~~~~~~~~~~~~~~~

.. c:function:: void json_print(const json_t * __restrict json) 

    | print json

    Parameters:
      | *[in]*  **json** json object with title and zero padding

.. c:function:: void json_print_pad(const json_t * __restrict json, int pad) 

    | print json

    Parameters:
      | *[in]*  **json** json object
      | *[in]*  **pad**  padding