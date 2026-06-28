#include <kv.h>
#include <string.h>

#define SEED 0x12a3e21a
#define TOMBSTONE (char*)0x1

kv_t *kv_init(size_t capacity) {
  kv_entry_t *entries = calloc(capacity, sizeof(kv_entry_t));
  if (entries == NULL) {
    return NULL;
  }

  kv_t *store = malloc(sizeof(kv_t));
  if (store == NULL) {
    free(entries);
    return NULL;
  }

  store->count = 0;
  store->capacity = capacity;
  store->entries = entries;

  return store;
}

size_t hash(const char *key, size_t capacity) {
  size_t hash = SEED;

  while (*key) {
    hash ^= *key;
    hash = hash << 8;
    hash += *key;

    key++;
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

    if (entry->key != NULL && entry->key != TOMBSTONE && strcmp(entry->key, key) != 0) {
      continue;
    }

    char *key_copy = strdup(key);
    char *value_copy = strdup(value);
    if (key_copy == NULL || value_copy == NULL) {
      free(key_copy);
      free(value_copy);
      return -3;
    }

    if (entry->key == NULL || entry->key == TOMBSTONE) {
      db->count++;
    } else {
      free(entry->key);
      free(entry->value);
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

    if (entry->key == TOMBSTONE || strcmp(entry->key, key) != 0) {
      continue;
    }

    return entry->value;
  }

  return NULL;
}

// Returns 0 on success, -1 on error
int kv_delete(kv_t *db, char *key) {
  if (db == NULL || key == NULL) {
    return -1;
  }

  size_t index = hash(key, db->capacity);

  for (size_t offset = 0; offset < db->capacity; offset++) {
    size_t db_index = (index + offset) % db->capacity;
    kv_entry_t *entry = &db->entries[db_index];

    if (entry->key == NULL) {
      return -1;
    }

    if (entry->key == TOMBSTONE || strcmp(entry->key, key) != 0) {
      continue;
    }

    free(entry->key);
    free(entry->value);

    entry->key = TOMBSTONE;
    entry->value = NULL;
    db->count--;
    return 0;
  }

  return -1;
}

void kv_free(kv_t *db) {
  if (db == NULL) {
    return;
  }

  for (size_t idx = 0; idx < db->capacity; idx++) {
    kv_entry_t *entry = &db->entries[idx];
    if (entry->key == NULL || entry->key == TOMBSTONE) {
      continue;
    }

    free(entry->key);
    free(entry->value);
  }

  free(db->entries);
  free(db);
}
