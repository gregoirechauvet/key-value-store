#include <stdio.h>
#include "inc/kv.h"

int main(int argc, char *argv[]) {
  kv_t *db = kv_init(16);
  printf("%p\n", db);
  printf("%ld\n", db->capacity);
  printf("%ld\n", db->count);

  int pif = kv_put(db, "key", "value");
  printf("Success %d\n", pif);
  printf("Count %ld\n", db->count);

  pif = kv_put(db, "key", "value2");
  printf("Success %d\n", pif);
  printf("Count %ld\n", db->count);

  pif = kv_put(db, "paf", "value");
  printf("Success %d\n", pif);
  printf("Count %ld\n", db->count);

  char *read = kv_get(db, "key");
  printf("Value read %s\n", read);
}
