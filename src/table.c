#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

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

static void adjustCapacity(HashTable *hashTable, int capacity) {
  Entry *entries = ALLOCATE(Entry, capacity);
  for (int i = 0; i < capacity; i++) {
    entries[i].key = NULL;
    entries[i].value = NIL_VAL;
  }

  hashTable->count = 0;
  for (int i = 0; i < hashTable->capacity; i++) {
    Entry *entry = &hashTable->entries[i];
    if (entry->key == NULL) continue;

    Entry *dest = findEntry(entries, capacity, entry->key);
    dest->key = entry->key;
    dest->value = entry->value;
    hashTable->count++;
  }

  FREE_ARRAY(Entry, hashTable->entries, hashTable->capacity);
  hashTable->entries = entries;
  hashTable->capacity = capacity;
}

bool findItem(HashTable *hashTable, ObjString *key, Value *value) {
  if (hashTable->count == 0) return false;

  Entry *entry = findEntry(hashTable->entries, hashTable->capacity, key);
  if (entry->key == NULL) return false;

  *value = entry->value;
  return true;
}

bool insertItem(HashTable *hashTable, ObjString *key, Value value) {
  if (hashTable->count + 1 > hashTable->capacity * TABLE_MAX_LOAD) {
    int capacity = GROW_CAPACITY(hashTable->capacity);
    adjustCapacity(hashTable, capacity);
  }
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
