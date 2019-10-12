#ifndef slang_debug_h
#define slang_debug_h

#include "chunk.h"

void disassembleChunk(Chunk *chunk, const char *name);
int disassembleInstruction(Chunk *chunk, int offset);

#endif /* slang_debug_h */
