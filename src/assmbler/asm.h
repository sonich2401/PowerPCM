#pragma once

#include "FileUtils.h"
#include "vector.h"
#include "types.h"

typedef struct{
  vector globals;
  fu_BinFile bin;
}Asm_file_t;

fu_BinFile assemble(char* __restrict__ file_path);
fu_BinFile assemble_s(fu_TextFile assembly);

