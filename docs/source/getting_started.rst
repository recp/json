Getting Started
================================

**json** uses **json_** prefix for all functions e.g. json_parse(). 
There are only a few types which represents json document, json object, json array and json value (as string).

- **json_doc_t** represents JSON document. It stores root JSON node and allocated memory.
- **json_t** represents JSON object. Arrays also are json object.
- **json_array_t** represents JSON array. It inherits **json_t**, so you can cast array to json object.
- **json_type_t** represents JSON type. 

Allocations:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*json* doesn't alloc any memory for JSON contents, keys and values... 
It ONLY allocs memory for DOM-tree (json tokens), that's it.

It creates pointers to actual data, so you must retain JSON data until you have finished to process json.

After you finished to parse JSON, this is the order that you must use to free-ing things:

1. free original JSON data
2. free json_doc 

actually the order doesn't matter but you must free the json doc which is returned from json_parse().

Design and Data Structure:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**json** creates a TREE to traverse JSON. Every json object's child node has **key** pointer. A value of **json_t** may be one of these:

- Child node
- String contents

you must use **type** member of json object to identify the value type. 
If you need to integer, float or boolean values, then you can use util functions to get these values.
These functions will be failed if the value is not string.

**VERY IMPORTANT:**

**key** and **value** ARE JUST POINTERS to original data. 
Because of this, you will see that json object has **keySize** and **valueSize** members.
When comparing two keys, you must use *keySize*. Instead of *strcmp()* you could use *strncmp()* and its friends, because it has *size* parameter which is our *keySize*

You can also use built-in helper to compare two keys: **json_key_eq()**

Also when copying values you must also use *valueSize*. You could use *json_string_dup()* to duplicate strings. It is better to not copy contents as possible as much.

**UTILITIES / HELPERS:**

json library also provides some inline utiliy functions to make things easier while handling json data.