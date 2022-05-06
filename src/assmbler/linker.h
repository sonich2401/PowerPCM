#pragma once
#include "types.h"
#include "cstr.h"
#include "FileUtils.h"

typedef struct{
    u64 ptr;
    cstr str;
}Label;

char str_is_label(cstr __restrict__ str);

Label Label_construct(char* __restrict__ str, u64 ptr);

void free_label(Label* lab);


fu_BinFile link_asm(fu_List file_List);
