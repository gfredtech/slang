#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

void initHashTable(HashTable *hashTable) {
  hashTable->count = 0;
  hashTable->capacity = 0;
  hashTable->entries = NULL;
}

void freeHashTable(HashTable *hashTable) {
  FREE_ARRAY(Entry, hashTable->entries, hashTable->capacity);
  initHashTable(hashTable);
}

static Entry *findEntry(Entry *entries, int capacity, ObjString *key) {
  uint32_t idx = key->hash % capacity;
  Entry *tombstone = NULL;
  for (;;) {
    Entry *entry = &entries[idx];

    if (entry->key == NULL) {
      if (IS_NIL(entry->value)) {
        return tombstone != NULL ? tombstone : entry;
      } else {
        // tombstone sentinel
        if (tombstone == NULL) tombstone = entry;
      }
    } else if (entry->key == key) {
      // valid entry
      return entry;
    }

    idx = (idx + 1) % capacity;
  }
}

bool findItem(HashTable *hashTable, ObjString *key, Value *value) {
  if (hashTable->count == 0) return false;

  Entry *entry = findEntry(hashTable->entries, hashTable->capacity, key);
  if (entry->key == NULL) return false;

  *value = entry->value;
  return true;
}

bool insertItem(HashTable *hashTable, ObjString *key, Value value) {
  Entry *entry = findEntry(hashTable->entries, hashTable->capacity, key);

  bool isNewKey = entry->key == NULL;
  if (isNewKey && IS_NIL(entry->value)) hashTable->count++;

  entry->key = key;
  entry->value = value;
  return isNewKey;
}

bool deleteItem(HashTable *hashTable, ObjString *key) {
  if (hashTable->count == 0) return false;

  Entry *entry = findEntry(hashTable->entries, hashTable->capacity, key);
  if (entry->key == NULL) return false;

  entry->key = NULL;
  entry->value = BOOL_VAL(true);

  return false;
}

void copyTable(HashTable *from, HashTable *to) {
  for (int i = 0; i < from->capacity; i++) {
    Entry *entry = &from->entries[i];
    if (entry->key != NULL) {
      insertItem(to, entry->key, entry->value);
    }
  }
}
