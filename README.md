# 🔋 JSON parser for `C`
[![Build Status](https://travis-ci.org/recp/json.svg?branch=master)](https://travis-ci.org/recp/json)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5c5ff7a0b2834889b7a83f7856112133)](https://www.codacy.com/app/recp/json?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=recp/json&amp;utm_campaign=Badge_Grade)

This is very simple and very powerful JSON parser. It creates DOM-like data structure and allows to iterate and process JSON objects very simple way.
It does not alloc any memory for JSON itself, it only allocs memory for tokens. It also does not use recursive way to build data structure which makes it very fast to build DOM-like tree structure.


## Features
-  doesn't alloc memory for keys and values only for tokens
-  creates DOM-like data structure to make it easy to iterate though
-  simple api
-  provides some util functions to print json, get int32, int64, float, double...
-  very small library

## Build

### Unix (Autotools)

```bash
$ sh autogen.sh
$ ./configure
$ make
$ [sudo] make install
```

you can grap library in .libs folder after build finished

### Windows (MSBuild)
Windows related build files, project files are located in `win` folder,
make sure you are inside `tm/win` folder.
Code Analysis are enabled, it may take awhile to build

```Powershell
$ cd win
$ .\build.bat
```

### Cmake
todo.

#### Example usage

```C
#include <json/json.h>
#include <json/print.h>

int main(int argc, const char * argv[]) {
  json_doc_t *doc;
  
  doc = json_parse(/* JSON string */);

  /* you can inspect json_print() to view usage in more detail */
  json_print(doc->root);

  json_free(doc);

  return 0;
}

```

## License
MIT. check the LICENSE file
