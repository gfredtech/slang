#ifndef slang_hashtable_h
#define slang_hashtable_h

#include "common.h"
#include "value.h"

typedef struct {
  ObjString *key;
  Value value;
} Entry;

typedef struct {
  int count;
  int capacity;
  Entry *entries;
} HashTable;

void initHashTable(HashTable *hashTable);
void freeHashTable(HashTable *hashTable);
bool findItem(HashTable *hashTable, ObjString *key, Value *value);
bool insertItem(HashTable *hashTable, ObjString *key, Value value);
bool deleteItem(HashTable *table, ObjString *key);
void copyTable(HashTable *from, HashTable *to);
ObjString *tableFindString(HashTable *table, const char *chars, int length,
                           uint32_t hash);

#endif /* slang_hashtable_h */
