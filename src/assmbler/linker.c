#include "linker.h"
#include "asm.h"
#include "FileUtils.h"

#include <malloc.h>
#include <string.h>

inline char str_is_label(cstr __restrict__ str){
    return (strstr(str, ":") != NULL && strstr(str, "\"") ==NULL);
}

inline Label Label_construct(char* __restrict__ str, u64 ptr){
    Label ret;
    ret.ptr = ptr;
    ret.str = str;
    return ret;
}

inline void free_label(Label* __restrict__ lab){
  free(lab->str);
}


inline fu_BinFile link_asm(fu_List file_list){
  fu_BinFile ret;
  
  fu_TextFile root = fu_load_text_file(file_list.text[0]);
  for(fu_index  i = 1; i < file_list.size; i++){
    fu_TextFile tmp = fu_load_text_file(file_list.text[i]);
    fu_join_list((fu_List*)&root, tmp);
    fu_free_list(tmp);
  }

  ret = assemble_s(root);
  
  fu_free_text_file(root);
  return ret;
}
