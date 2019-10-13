#ifndef slang_object_h
#define slang_object_h

#include "common.h"
#include "value.h"

// clang-format off
#define OBJ_TYPE(value)      (AS_OBJ(value)->type)

#define IS_STRING(value)     isObjType(value, OBJ_STRING)

#define AS_STRING(value)     ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value)    ((ObjString *)AS_OBJ(value)->chars)
// clang-format on

typedef enum {
  OBJ_STRING,
} ObjType;

struct sObj {
  ObjType type;
};

struct sObjString {
  Obj obj;
  int length;
  char *chars;
};

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif /* slang_object_h */
