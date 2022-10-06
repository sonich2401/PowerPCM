#pragma once

#include "FileUtils.h"
#include "vector.h"
#include "types.h"

typedef struct{
  vector labels;
  vector globals;
  fu_BinFile bin;
}Asm_file_t;

fu_BinFile assemble(const char* __restrict__ file_path);
fu_BinFile assemble_s(fu_TextFile assembly);

Asm_file_t assemble_obj(const char* __restrict file_path, vector* globals);
Asm_file_t assemble_obj_s(fu_TextFile assembly, vector* globals);