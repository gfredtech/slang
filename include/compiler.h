#ifndef slang_compiler_h
#define slang_compiler_h

#include "object.h"
#include "vm.h"

ObjFunction *compile(const char *source);

#endif /* slang_compiler_h */
