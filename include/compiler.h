#ifndef slang_compiler_h
#define slang_compiler_h

#include "object.h"
#include "vm.h"

bool compile(const char *source, Chunk *chunk);

#endif /* slang_compiler_h */
