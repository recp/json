#include "../include/json/json.h"
#include "../include/json/print.h"
#include <stdio.h>


int main(int argc, const char * argv[]) {
  unsigned char buf[8192];
  json_doc_t *doc;
  json_t     *root;
  
  fread(buf, 1, sizeof(buf) - 1, stdin);
  buf[sizeof(buf) - 1] = 0;
  doc  = json_parse(buf, true);
  root = doc->root;

  json_print_human(stderr, root);

  json_free(doc);

  return 0;
}
 
