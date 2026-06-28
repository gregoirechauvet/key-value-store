#include <kv.h>
#include <string.h>

#define SEED 0x12a3e21a

kv_t *kv_init(size_t capacity) {
  kv_entry_t *entries = calloc(sizeof(kv_entry_t), capacity);

  kv_t *store = malloc(sizeof(kv_t));
  store->count = 0;
  store->capacity = capacity;
  store->entries = entries;

  return store;
}

size_t hash(const char *value, size_t capacity) {
  size_t hash = SEED;

  while (*value) {
    hash ^= *value;
    hash = hash << 8;
    hash += *value;

    value++;
  }

  return hash % capacity;
}

// Returns 0 on success, -1 on invalid inputs, -2 if datastore is full, -3 on memory error
int kv_put(kv_t *db, char *key, char *value) {
  if (db == NULL || key == NULL || value == NULL) {
    return -1;
  }

  size_t index = hash(key, db->capacity);

  for (size_t offset = 0; offset < db->capacity; offset++) {
    size_t db_index = (index + offset) % db->capacity;
    kv_entry_t *entry = &db->entries[db_index];

    if (entry->key != NULL && strcmp(entry->key, key) != 0) {
      continue;
    }

    if (entry->key == NULL) {
      db->count++;
    } else {
      free(entry->key);
      free(entry->value);
    }

    char *key_copy = strdup(key);
    char *value_copy = strdup(value);
    if (key_copy == NULL || value_copy == NULL) {
      free(key_copy);
      free(value_copy);
      return -3;
    }

    entry->key = key_copy;
    entry->value = value_copy;
    return 0;
  }

  return -2;
}

char *kv_get(kv_t *db, char *key) {
  if (db == NULL || key == NULL) {
    return NULL;
  }

  size_t index = hash(key, db->capacity);

  for (size_t offset = 0; offset < db->capacity; offset++) {
    size_t db_index = (index + offset) % db->capacity;
    kv_entry_t *entry = &db->entries[db_index];

    if (entry->key == NULL) {
      return NULL;
    }

    if (strcmp(entry->key, key) != 0) {
      continue;
    }

    return entry->value;
  }

  return NULL;
}
