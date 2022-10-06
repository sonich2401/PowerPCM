#include "linker.h"
#include "asm.h"
#include "FileUtils.h"
#include "../bus.h"

#include <malloc.h>
#include <string.h>

u64 pc = 0; //Used for linking and assembling


#define PUSH_LABEL(key, val) tmp_label = Label_construct(strdup(key), val); vector_push_back(&ret, &tmp_label)

vector get_predefined_labels(){
  vector ret;
  vector_init(&ret, sizeof(Label), free_label);
  Label tmp_label;
  
  PUSH_LABEL("__STACK_TOP__", STACK_SIZE);
  PUSH_LABEL("___MALLOC__BLOCK__", STACK_SIZE);
  
  PUSH_LABEL("true", 1);
  PUSH_LABEL("TRUE", 1);
  PUSH_LABEL("True", 1);
  
  PUSH_LABEL("false", 0);
  PUSH_LABEL("FALSE", 0);
  PUSH_LABEL("False", 0);
  
  PUSH_LABEL("NULL", 0);
  PUSH_LABEL("null", 0);
  PUSH_LABEL("nullptr", 0);
  
  //ret.push_back(&ret, &tmp_label);
  return ret;
}

#undef PUSH_LABEL


INLINE char str_is_label(cstr __restrict__ str){
  return (strstr(str, ":") != NULL && strstr(str, "\"") ==NULL);
}

Label INLINE Label_construct(char* __restrict__ str, u64 ptr){
    Label ret;
    ret.ptr = ptr;
    ret.str = str;
    return ret;
}

INLINE void free_label(Label* __restrict__ lab){
  free(lab->str);
}


INLINE fu_BinFile link_asm(fu_List file_list){
  fu_BinFile ret;
  ret.bin = NULL;
  pc = 0;
  
  vector globals = get_predefined_labels();
  for(fu_index  i = 0; i < file_list.size; i++){
    Asm_file_t tmp = assemble_obj(file_list.text[i], &globals);
    vector_merge(&globals, &tmp.globals);
    if(ret.bin == NULL){
      ret = tmp.bin;
    } else {
      fu_join_bin(&ret, tmp.bin);
    }
  }
  printf("%p %X\n", ret.bin, ret.size);

  return ret;
}
