# 🔋 JSON parser for `C`

[![Build Status](https://travis-ci.org/recp/json.svg?branch=master)](https://travis-ci.org/recp/json)
[![Build status](https://ci.appveyor.com/api/projects/status/c2lm67q37u2s2gu2/branch/master?svg=true)](https://ci.appveyor.com/project/recp/json/branch/master)
[![Documentation Status](https://readthedocs.org/projects/json/badge/?version=latest)](https://json.readthedocs.io/en/latest/?badge=latest)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5c5ff7a0b2834889b7a83f7856112133)](https://www.codacy.com/app/recp/json?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=recp/json&amp;utm_campaign=Badge_Grade)

This is very simple and very powerful JSON parser. It creates DOM-like data structure and allows to iterate and process JSON objects very simple way.
It does not alloc any memory for JSON itself, it only allocs memory for tokens. It also does not use recursive way to build data structure which makes it very fast to build DOM-like tree structure.

#### Documentation

Almost all functions (inline versions) and parameters are documented inside related headers. <br />
Complete documentation: http://json.readthedocs.io

## Features

- header-only or optional compiled library
- option to store members and arrays as reverse order or normal
- doesn't alloc memory for keys and values only for tokens
- creates DOM-like data structure to make it easy to iterate though
- simple api
- provides some util functions to print json, get int32, int64, float, double...
- very small library
- unique way to parse JSON (check the object map section)

#### Object Map

Here a VERY UNIQUE and VERY COOL and VERY EASY and VERY FAST way to parse known JSON: objmap. 

```C
void
callback_1(json_t * __restrict json, void * __restrict obj) {
  printf("entered callback_1\n");
}

json = json_parse(/* JSON string */, true);

json_objmap_t objmap[] = {
    {
      .key = "key1",
      .foundFunc = {
        .func  = callback_1,
        .param = "callback 1 param" 
      }
    },
    {
      .key = "key2",
      .foundFunc = {
        .func = callback_1
      }
    }
};

/* or you can use macro helpers which is more readable if you don't need more details: */
json_objmap_t objmap[] = {
    JSON_OBJMAP_FN("key 1", func1, param1),
    JSON_OBJMAP_FN("key 2", func2, param2),
    JSON_OBJMAP_FN("key 3", func3, param3),
    /* ... */
};

json_objmap_call(json, objmap, ARRAY_LEN(objmap), NULL);

/* or use this to iterate objmap manually */
json_objmap(json, objmap, ARRAY_LEN(objmap));
```

In this way you don't have to compare keys in a loopi just map the keys with a function or with userdata. You don't have to use function in this way, you may use to map json object to userdata which may be a GOTO LABEL (to use compound gotos) or something else. 

#### Important Note for Arrays

- **json_array_t** inherits **json_t**, so you can cast **json_array_t** to **json_t**. It only adds **count** member.
- The **value** link gives array's childen (last element if you set `reverse` to true)
- Every item of **json_array_t** is **json_t**. You can get integer, float or boolean values from that object. The **next** link of that object is next item of that object in array.

#### Important Note for keys and values

- json doesn't copy keys and values, it only gives pointers to key and values. So when compaing keys or copying values, you must use keySize or valSize. Or you can use builtin inline functions.

## TODOs

- [x] provide header only library and optionally compile version
- [x] provide option to preserve array order (currently array order is reversed, because it is easy to parse it in this way; this may be changed. Please follow new commits or releases)
- [x] windows build
- [x] documentation
- [ ] ignore comments?
- [x] cmake
- [ ] tests
- [ ] extra optimizations
- [ ] usage in detail
- [ ] Unicode support (UTF-8)
- [ ] null object

## Build

### Unix (Autotools)

```bash
sh autogen.sh
./configure
make
[sudo] make install
```

you can grap library in .libs folder after build finished

### Windows (MSBuild)

Windows related build files, project files are located in `win` folder,
make sure you are inside `tm/win` folder.
Code Analysis are enabled, it may take awhile to build

```Powershell
cd win
.\build.bat
```

##### Cmake options with Defaults:

```CMake
option(JSON_SHARED "Shared build" ON)
option(JSON_STATIC "Static build" OFF)
option(JSON_USE_C99 "" OFF) # C11 
option(JSON_USE_TEST "Enable Tests" OFF) # for make check - make test
```

```bash
mkdir build && cd build
cmake ..
make
[sudo] make install
```

### Header-only or Compiled Library

The functions has the `jsonc_` prefix are compiled version which is called from library. To use this feature you must include `json/call/json.h` header.

To use header-only library you must include `json/json.h` header. The functions has the `json_` prefix are forced to be inlined. When you use this, you don't have to compile the library.

todo.

#### Example usage

You can inspect `json_print()` to view usage in more detail. The example will be updated later to give more detail.

```C
#include <json/json.h>
#include <json/print.h>

int main(int argc, const char * argv[]) {
  json_doc_t *doc;
  json_t     *root;
  
  doc  = json_parse(/* JSON string */, true);
  root = doc->root;

  json_print_human(stderr, root);

  json_free(doc);

  return 0;
}

```

```C

const json_doc_t *jsonDoc;
const json_t         *json;

jsonDoc = json_parse(/* JSON string */, true);
json    = jsonDoc->root->value;

/* already defined in util.h */
JSON_INLINE
bool
json_key_eq(const json_t * __restrict obj, const char * __restrict str) {
  return strncmp(str, obj->key, obj->keySize) == 0;
}

while (json) {
    if (json_key_eq(json, "key 1")) {
      int aNumber;

     aNumber = json_int32(json, 0);

     /* ... */
    } else if (json_key_eq(json, "key 2")) {
      const char *nonNullTerminatedString;
      const char *nullTerminatedString;

      /* just pointer */
      nonNullTerminatedString = json_string(json);

       /* null-terminated string (strdup), needs to be freed */
      nullTerminatedString    = json_string_dup(json);

     /* ... */
    } else if (json_key_eq(json,"key 3")) {
      json_t *aChild;
      
      aChild = json->value;
      while (aChild) {
          /* handle child node */
          aChild = aChild->next;
      }
    }

    json = json->next;
}
```

#### Using Object Map

Here a VERY UNIQUE and VERY COOL and VERY EASY and VERY FAST way to parse known JSON: objmap. 

```C
void
callback_1(json_t * __restrict json, void * __restrict obj) {
  printf("entered callback_1\n");
}

json = json_parse(/* JSON string */, true);

json_objmap_t objmap[] = {
    {
      .key = "key1",
      .foundFunc = {
        .func = callback_1
      }
    },
    {
      .key = "key2",
      .foundFunc = {
        .func = callback_1
      }
    }
};

/* or you can use macro helpers which is more readable if you don't need more details: */
json_objmap_t objmap[] = {
    JSON_OBJMAP_FN("key 1", func1, param1),
    JSON_OBJMAP_FN("key 2", func2, param2),
    JSON_OBJMAP_FN("key 3", func3, param3),
    /* ... */
};

json_objmap_call(json, objmap, ARRAY_LEN(objmap), NULL);
```

In this way you don't have to compare keys in a loopi just map the keys with a function or with userdata. You don't have to use function in this way, you may use to map json object to userdata which may be a GOTO LABEL (to use compound gotos) or something else. 

## License

MIT. check the LICENSE file
