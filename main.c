#include <stdio.h>
#include "inc/kv.h"

int main(int argc, char *argv[]) {
  kv_t *db = kv_init(16);
  printf("%p\n", db);
  printf("%ld\n", db->capacity);
  printf("%ld\n", db->count);
}
