#ifndef slang_value_h
#define slang_value_h

#include "common.h"

typedef double Value;

typedef struct {
  int capacity;
  int count;
  Value *values;
} ValueArray;

void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
void freeValueArray(ValueArray *array);

#endif /* slang_value_h */
