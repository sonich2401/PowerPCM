#pragma once
#include "types.h"
#include "cstr.h"
#include "FileUtils.h"
#include "vector.h"

typedef struct{
    u64 ptr;
    cstr str;
}Label;

char str_is_label(cstr __restrict__ str);

Label Label_construct(char* __restrict__ str, u64 ptr);

void free_label(Label* lab);


#define PUSH_LABEL(key, val) tmp_label = Label_construct(strdup(key), val); vector_push_back(&ret, &tmp_label)

vector get_predefined_labels();

extern u64 pc;

fu_BinFile link_asm(fu_List file_List);

//vector 