#include <kv.h>

kv_t *kv_init(size_t capacity) {
  kv_entry_t *entries = calloc(sizeof(kv_entry_t), capacity);

  kv_t *store = malloc(sizeof(kv_t));
  store->count = 0;
  store->capacity = capacity;
  store->entries = entries;

  return store;
}
