#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cstr.h"
#include "vector.h"
#include "types.h"
#include "../opcode.h"
#include "../bus.h"

#include "linker.h"
#include "asm.h"


#if 0

#define TYPE_0(opcode) return opcode;
#define TYPE_A(opcode) return SET_REG(SET_REG(SET_REG(opcode, 1, dest), 2, a), 3, b);
#define TYPE_B(opcode) return SET_ADD_IMM(SET_REG(SET_REG(opcode, 1, dest), 2, a), imm);
#define TYPE_C(opcode) return SET_IMM(SET_REG(SET_REG(opcode, 1, dest), 2, a), imm);
#define TYPE_D(opcode) return SET_IMM(SET_REG(opcode, 1, dest), imm);
#define TYPE_E(opcode) return SET_REG(opcode, 1, dest);
#define TYPE_F(opcode) return SET_IMM_IMP(opcode, imm);

#else

#define TYPE_0(opcode) return opcode;

#define TYPE_A(opcode) u32 ret = SET_REG(opcode, 1, dest);\
ret = SET_REG(ret, 2, a);\
ret = SET_REG(ret, 3, b);\
return ret;

#define TYPE_B(opcode) u32 ret = SET_REG(opcode, 1, dest);\
ret = SET_REG(ret, 2, a);\
ret = SET_ADD_IMM(ret, imm);\
return ret;

#define TYPE_C(opcode) u32 ret = SET_REG(opcode, 1, dest);\
ret = SET_REG(ret, 2, a);\
ret = SET_INDEX_OFFSET(ret, imm); \
return ret;

  
#define TYPE_D(opcode) u32 ret = SET_REG(opcode, 1, dest);\
ret = SET_IMM(ret, imm);\
return ret;

#define TYPE_E(opcode) return SET_REG(opcode, 1, dest);

#define TYPE_F(opcode) return SET_IMM_IMP(opcode, imm);


#endif



static INLINE u32 func_A(u8 opcode, u8 dest, u8 a, u8 b){
  TYPE_A(opcode);
}

static INLINE u32 func_B(u8 opcode, u8 dest, u8 a, u32 imm){
  TYPE_B(opcode);
}

static INLINE u32 func_C(u8 opcode, u8 dest, u32 imm, u8 a){
  TYPE_C(opcode);
}

static INLINE u32 func_D(u8 opcode, u8 dest, u32 imm){
  TYPE_D(opcode);
}

static INLINE u32 func_E(u8 opcode, u8 dest){
  TYPE_E(opcode);
}

static INLINE u32 func_F(u8 opcode, u32 imm){
  TYPE_F(opcode);
}

static INLINE u32 func_0(u8 opcode){
  TYPE_0(opcode);
}

#define func_mul func_A
#define func_muli func_B
#define func_div func_A
#define func_divi func_B
#define func_sub func_A
#define func_subi func_B
#define func_add func_A
#define func_addi func_B

#define func_and func_A
#define func_andi func_B
#define func_or func_A
#define func_ori func_B
#define func_xor func_A
#define func_xori func_B
#define func_not func_F

#define func_li func_D
#define func_load_byte func_C
#define func_load_halfword func_C
#define func_load_word func_C
#define func_sb func_C
#define func_shw func_C
#define func_sw func_C

#define func_sl func_A
#define func_sli func_B
#define func_sr func_A
#define func_sri func_B

#define func_b func_F
#define func_bl func_F
#define func_beq func_F
#define func_bne func_F
#define func_blt func_F
#define func_ble func_F
#define func_bgt func_F
#define func_bge func_F
#define func_blr func_0

#define func_bdz func_F
#define func_bdnz func_F
#define func_bctrl func_0

#define func_cmp func_A
#define func_cmpi func_D

#define func_mr func_A
#define func_syscall func_F
#define func_mtlr func_E
#define func_mflr func_E
#define func_mtctr func_E
#define func_mfctr func_E
#define func_end func_F

#define func_nop func_0


typedef u32 (*asm_microcode_t)(u32, u32, u32, u32, u32);

asm_microcode_t asm_opcode_ptr[0b111111] = {func_nop};


#define DEF_OP(opcode, func) asm_opcode_ptr[opcode] = func;
static inline void init_ops(){
  for(u8 i = 0; i < 0b111111; i++){
    asm_opcode_ptr[i] = NULL;
  }
  DEF_OP(NOP, func_nop);
  
  DEF_OP(ADD, func_add);
	DEF_OP(ADDI, func_addi);
	
	DEF_OP(LI, func_li);
	DEF_OP(LW, func_load_word);
	DEF_OP(LB, func_load_byte);
	DEF_OP(LHW, func_load_halfword);
	
	DEF_OP(SW, func_sw);
	DEF_OP(SB, func_sb);
	DEF_OP(SHW, func_shw);
	
	DEF_OP(MUL, func_mul);
	DEF_OP(MULI, func_muli);
	
	DEF_OP(DIV, func_div);
	DEF_OP(DIVI, func_divi);
	
	DEF_OP(SUB, func_sub);
	DEF_OP(SUBI, func_subi);
	
	DEF_OP(CMP, func_cmp);
	DEF_OP(CMPI, func_cmpi);
	
	DEF_OP(B, func_b);
	DEF_OP(BL, func_bl);
	DEF_OP(BLR, func_blr);
	DEF_OP(BEQ, func_beq);
	DEF_OP(BNE, func_bne);
	DEF_OP(BLT, func_blt);
	DEF_OP(BLE, func_ble);
	DEF_OP(BGT, func_bgt);
	DEF_OP(BGE, func_bge);
	
	DEF_OP(BDZ, func_bdz);
	DEF_OP(BDNZ, func_bdnz);
	DEF_OP(BCTRL, func_bctrl);
	
	DEF_OP(SR, func_sr);
	DEF_OP(SRI, func_sri);
	DEF_OP(SL, func_sl);
	DEF_OP(SLI, func_sli);
	
	DEF_OP(AND, func_and);
	DEF_OP(ANDI, func_andi);
	DEF_OP(OR, func_or);
	DEF_OP(ORI, func_ori);
	DEF_OP(XOR, func_xor);
	DEF_OP(XORI, func_xori);
	DEF_OP(NOT, func_not);
	
	DEF_OP(MR, func_mr);
	DEF_OP(MFLR, func_mflr);
	DEF_OP(MTLR, func_mtlr);
	DEF_OP(MFCTR, func_mfctr);
	DEF_OP(MTCTR, func_mtctr);
	DEF_OP(SC, func_syscall);
	DEF_OP(END, func_end);
	
	
  extern char* opcode_str[OPCODE_LEN];
	 for(u8 i = 0; i < OPCODE_LEN; i++){
    if(asm_opcode_ptr[i] == NULL && opcode_str[i] != NULL){
      fprintf(stderr, "WARNING ASM: opcode %u \"%s\" not implimented\n", i, opcode_str[i]);
    }
  }
}



extern char* opcode_str[OPCODE_LEN];

u8 name_to_instr(const char* __restrict__ memonic){
  char* name = cstrdup_stack(memonic);
  char* ogptr = name;
  //Normalize name
  while(*name != 0){
    if(*name == ' ') {
      *name = 0; 
      break;
    }
    if(*name > 0x5F){
      *name -= ('a' - 'A');
    }

    name++;
  }
  name = ogptr;
  
  //Search for opcode
  for(unsigned int i = 0; i < OPCODE_LEN; i++){
    if(strcmp(name, opcode_str[i]) == 0){
      return i;
    }
  }

  fprintf(stderr, "ERR: Unknown memonic of \"%s\"\n", memonic);
  abort();
  return 0;
}




//Add opcode to bin and then move the cursor to the next location
static INLINE char* push32(char* __restrict__ buff, u32 opdat){
  *((u32*)buff) = opdat;
  buff += sizeof(u32);
  return buff;
}

#define PUSH32(val) tmp_bin = push32(tmp_bin, val);


//finds '\' 'n' and converts it to the single character of '\n'
static void decode_backslashes(cstr str){
  cstr og_string = str;
  while((str = strchr(str, '\\')) != NULL){
    cstrshl(str, 1); //Remove \ character
    
    switch(*str){
      case 'n':
        *str = '\n';
      break;
      case 'b':
        *str = '\b';
      break;
      case 't':
        *str = '\t';
      break;
      case 'r':
        *str = '\r';
      break;
      case '0':
        cstrshl(str, 1); //Count as a spacebar
      break;
      
      default:
        fprintf(stderr, "ERR ASM: Current string contains a special character that I do not know how to interperut! str = \"%s\"\n", og_string);
        abort();
      break;
    }
  }
}



//Possible future use for a proper linker?
typedef struct{
  vector labels;
  Asm_file_t asm_global;
}Intermediate_Asm_File_t;

static u64 pc; //Used to find where to place the data
extern char logging;

static Intermediate_Asm_File_t get_labels(fu_TextFile* txt, vector* __restrict__ predefined){
  pc = 4; //For some reason it needs to be 8 instead of 4?
  u64 start_pc = 0;//Set the rom start address to 0. If this stays zero after all labels are decoded then 'start' was never defined
  vector labels;
  vector globals = vector_create(sizeof(Label), free_label);
  
  //Add the predefined labels
  if(predefined){
    labels = *predefined;
  }else{
    labels = vector_create(sizeof(Label), free_label);
  }
  
  Intermediate_Asm_File_t ret;
  ret.asm_global.bin = fu_alloc_bin_file(sizeof(u32));
  
  for(u64 i = 0; i < txt->size; i++){
    char* cursor = strchr(txt->text[i], ':');
    if(!cursor) continue;
    if(cstrcnt(txt->text[i], "\"") == 2 && strstr(txt->text[i], ".file ") != txt->text[i]){
      //Remove quotes and copy binary data
      fu_BinFile dat_seg;
      dat_seg.bin = strdup(strchr(txt->text[i], '"') + 1);
      *(strrchr(dat_seg.bin, '"')) = 0;
      
      //Decode backslash characters
      decode_backslashes(dat_seg.bin);
      //Copy into executable binary
      dat_seg.size = strlen(dat_seg.bin) + 1;
      fu_join_bin(&ret.asm_global.bin, dat_seg);

      
      //Add label
      Label tmp = Label_construct(strdup(txt->text[i]), pc + ROM_START); //Add +ROM_START because DMA is not relitive to cpu.pc
      *(strchr(tmp.str, ' ') - 1) = 0; //remove ':' character
      labels.push_back(&labels, &tmp);
      pc += dat_seg.size;
      fu_delete_text(txt, i); //Remove line that contained the label
      i--;
      
      fu_free_bin_file(dat_seg);
      continue;
    }
  
    //.file testFile: "/home/rendev/Desktop/myFile.s"
    if(strlen(cursor) > 2 && strstr(txt->text[i], ".file ") == txt->text[i]){  //Number is after the label
      if(cstrcnt(txt->text[i], "\"") != 2){
        fprintf(stderr, "ERR: Line #%llu: \"%s\" does not have double quotes for the file name!\n", i, txt->text[i]);
        exit(EXIT_FAILURE);
      }

      char* fp = cstrdup_stack(strchr(txt->text[i], '"') + 1);
      *(strrchr(fp, '"')) = 0;
      char* tmp_cursor = strstr(fp, ".png");
      bool isSprite = false;
      if(tmp_cursor != NULL){
        isSprite = strlen(tmp_cursor) == strlen(".png"); //Does the file have a .png exension
      }

      fu_BinFile bin = fu_load_bin_file(fp);

      //add the file byte count to the constant data
      fu_BinFile dat_seg = fu_alloc_bin_file(sizeof(fu_index));
      *(fu_index*)dat_seg.bin = bin.size;

      fu_join_bin(&ret.asm_global.bin, dat_seg);
      fu_join_bin(&ret.asm_global.bin, bin);


      
      //Add label
      Label tmp = Label_construct(strdup(strchr(txt->text[i], ' ') + 1), pc + ROM_START); //Add +ROM_START because DMA is not relitive to cpu.pc
      *(strchr(tmp.str, ' ') - 1) = 0; //remove ':' character
      labels.push_back(&labels, &tmp);
      pc += dat_seg.size;
      pc += bin.size;
      fu_delete_text(txt, i); //Remove line that contained the label
      i--;

      fu_free_bin_file(dat_seg);
      fu_free_bin_file(bin);
      continue;
    }

    //.macro testMacro: 20
    if(strlen(cursor) > 2 && strstr(txt->text[i], ".macro ") == txt->text[i]){  //Number is after the label

      unsigned int tmpint = 0;
      sscanf(cursor + 1, "%u", &tmpint);
      
      //Add label
      Label tmp = Label_construct(strdup(strchr(txt->text[i], ' ') + 1), tmpint);
      *(strchr(tmp.str, ' ') - 1) = 0; //remove ':' character
      labels.push_back(&labels, &tmp);
      fu_delete_text(txt, i); //Remove line that contained the label
      i--;
      
      continue;
    }
    
    //testInt: 210
    if(strlen(cursor) > 2){  //Number is after the label
      
      fu_BinFile dat_seg;
      dat_seg.bin = (char*)malloc(sizeof(unsigned int));
      sscanf(cursor + 1, "%u", (unsigned int*)dat_seg.bin);
      
      //Copy into executable binary
      dat_seg.size = sizeof(unsigned int);
      fu_join_bin(&ret.asm_global.bin, dat_seg);

      
      //Add label
      Label tmp = Label_construct(strdup(txt->text[i]), pc + ROM_START); //Add +ROM_START because DMA is not relitive to cpu.pc
      *(strchr(tmp.str, ' ') - 1) = 0; //remove ':' character
      labels.push_back(&labels, &tmp);
      pc += dat_seg.size;
      fu_delete_text(txt, i); //Remove line that contained the label
      i--;
      
      fu_free_bin_file(dat_seg);
      continue;
    }
  }
  
  fu_index data_size = pc;

  
  for(u64 i = 0; i < txt->size; i++){
    pc = i * 4 + data_size;
    if(strstr(txt->text[i], ":") != NULL){
      Label tmp = Label_construct(strdup(txt->text[i]), pc);
      tmp.str[strlen(tmp.str) - 1] =0;
      if(logging){
        printf("LABEL: \"%s\": %llX\n", tmp.str, tmp.ptr);
      }
      labels.push_back(&labels, &tmp);
      //printf("LBL \"%s\" at %llu\n", tmp.str, pc);
      if(strcmp(tmp.str, "start") == 0){
        start_pc = pc;
      }
      fu_delete_text(txt, i);
      i--;
      continue;
    }
    #if 0
    if(strstr(txt->text[i], ".global") != NULL){
      Label tmp = Label_construct(strdup(txt->text[i]), pc);
      tmp.str[strlen(tmp.str) - 1] = 0;
      globals.push_back(&globals, &tmp);
      fu_delete_text(txt, i);
      i--;
      continue;
    }
    #endif
    
    //This line of the file was assembly code
    
  }
  
  if(start_pc == 0){
    fprintf(stderr, "ASM ERR: 'start' label was not found!\n");
    abort();
  }else{
    *((u32*)ret.asm_global.bin.bin)  = ((u32)(start_pc)); //Set start rom address
  }

  ret.labels = labels;
  ret.asm_global.globals = globals;
  return ret;
}

static void remove_register_indicators(cstr str){
  if(!str) return;
  cstr tmp = str;
   
  while((tmp = strstr(str, "sp"))){
    //Convert sp to r0
    tmp[0] = 'r';
    tmp[1] = '0';
  }

  while((tmp = strchr(str, 'r')) != NULL){
    if(!(tmp[1] >= '0' && tmp[1] <= '9')) break; //Invalid characters found! isBadString will resolve this later
    cstrshl(tmp, 1);  //Remove the 'r'
  }

  while((tmp = strchr(str, 'f')) != NULL){
    cstrshl(tmp, 1);  //Remove the 'f'
  }
  
  while((tmp = strchr(str, '$')) != NULL){
    cstrshl(tmp, 1);  //Remove the '$'
  }
  
  while((tmp = strchr(str, '%')) != NULL){
    cstrshl(tmp, 1);  //Remove the '%'
  }
}


//Checks the string for any non number characters
static bool isBadString(cstr str){
	if(!(str = strchr(str, ' ')) ) return false; //Checks for the first spacebar. If there is no spacebar it returns false
	while(*str != 0){
		if(*str >= 'a' && *str <= 'z') return true;
		if(*str >= 'a' && *str <= 'z') return true;
		if(*str == '-') return true; //Disable negitive numbers for now
		if(!(*str >= '0' && *str <= '9') && (*str != ' ')) return true;
		str++;
	}
	return false;
}


static void clean_text(fu_TextFile* t){
  for(fu_index i = 0; i < t->size; i++){
    //Remove unwanted text
    char* tmp = strrchr(t->text[i], '(');
    if(tmp) *tmp = ' ';
    cstrrem(t->text[i], ',');
    cstrrem(t->text[i], ')');
    char* new_str = cstrrep(t->text[i], "(", " ");
    free(t->text[i]);
    t->text[i] = new_str;
    tmp = t->text[i];
    while((tmp = strstr(t->text[i], "  "))){ //remove all double spaces
      char* cursor = tmp;
      unsigned long char_count = 0;
      while(*cursor == ' ') {char_count++; cursor++;}
      cstrshl(tmp, char_count - 1); //remove all but 1 space
    }
    while(t->text[i][0] == ' '){ //Remove spaces from begining
      cstrshl(t->text[i], 1);
    }
    while(strrchr(t->text[i], '\t')){ //remove all tabs
      cstrrem(t->text[i], '\t');
    }
    while(strrchr(t->text[i], '\n')){ //remove all newlines (should never happen)
      cstrrem(t->text[i], '\n');
    }
    while(strrchr(t->text[i], '\r')){ //remove all return carrages (windows compatibility)
      cstrrem(t->text[i], '\r');
    }
    tmp = strchr(t->text[i], ';'); //Remove comments
    if(tmp != NULL){
      *tmp = 0;
    }

    //remove extra spaces at the end of text
    unsigned long text_len = strlen(t->text[i]);
    if(text_len > 0){ 
      while(t->text[i][text_len - 1] == ' '){
        t->text[i][text_len - 1] = 0;
        text_len--;
        if(text_len == 0) break;
      }
    }

    if(strlen(t->text[i]) == 0){ //Remove blank lines
      fu_delete_text(t, i);
      i--; // i - 1 + 1 = i
      continue;
    }
    //puts(t->text[i]);
  }
}

char* check_and_replace_label(cstr ele, Intermediate_Asm_File_t asm_f) {
  #define labels asm_f.labels
  //replace label with memory address
  for (u64 i1 = 0; i1 < labels.size(&labels); i1++) {
    Label* ele1 = (Label*)labels.index(&labels, i1);
    char* cache = strstr(ele, ele1->str);
    if (cache != NULL && *(cache - 1) == ' ') {
      char* tmp_string = cstrdup_stack(cache);
      
      char* helper;
      if ((helper = strchr(tmp_string, ' '))) {
        *helper = 0;
      }

      if (strcmp(tmp_string, ele1->str) != 0) {
        continue;
      }
      
      char num_buffer[50];
      sprintf(num_buffer, "%llu", ele1->ptr);
      ele = cstrrep(ele, ele1->str, num_buffer);
      break;
    }
  }
  #undef labels
  return ele;
}


#define PUSH_LABEL(key, val) tmp_label = Label_construct(strdup(key), val); ret.push_back(&ret, &tmp_label)

static vector get_predefined_labels(){
  vector ret;
  vector_init(&ret, sizeof(Label), free_label);
  Label tmp_label;
  
  PUSH_LABEL("__STACK_TOP__", STACK_SIZE);
  PUSH_LABEL("___MALLOC__BLOCK__", STACK_SIZE);
  PUSH_LABEL("__FRAMEBUFFER_START__", FRAMEBUFFER_START);
  PUSH_LABEL("__FRAMEBUFFER_END__", FRAMEBUFFER_END);
  PUSH_LABEL("__FRAMEBUFFER_WIDTH__", SCREEN_W);
  PUSH_LABEL("__FRAMEBUFFER_HEIGHT__", SCREEN_H);
  
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





//MAIN ASSEMBLING CODE


fu_BinFile assemble_s(fu_TextFile assembly){
  if(logging){
    puts("ASSEMBLING ...");
  }
  
  init_ops();
  
  fu_TextFile t = fu_create_text_copy(assembly);
  clean_text(&t); //Remove all junk characters
  
  vector predefined_labels = get_predefined_labels();
  Intermediate_Asm_File_t tmp_asm = get_labels(&t, &predefined_labels); //resolve all labels and macros
  
  //Alloc only nessisary size  
  fu_BinFile ret = fu_alloc_bin_file((tmp_asm.asm_global.bin.size + ((t.size + 2) * 4)) % ROM_SIZE);  
  char* tmp_bin = ret.bin;
  
  //Copy data segments into rom including start addr
  memcpy(ret.bin, tmp_asm.asm_global.bin.bin, tmp_asm.asm_global.bin.size);
  tmp_bin += tmp_asm.asm_global.bin.size;
  u64 start_addr = tmp_asm.asm_global.bin.size;
  
  
  for(fu_index i = 0; i < t.size; i++){
    //Replace label if it exists
    t.text[i] = check_and_replace_label(t.text[i], tmp_asm);
    
    //Remove register indicators (aka. r13 or $r32, or %r2)
    remove_register_indicators(strchr(t.text[i], ' '));

    #ifndef DISABLE_ERR_CHECKING
    if(isBadString(t.text[i])){
      fprintf(stderr, "ERR: Assembler: Line %llu of the file contains invalid characters!\nAssembly dump is as follows which shows the offending code ...\n", i);
      while((i   - 2) >= t.size){i++;}
      for(fu_index i1 = (i - 2); i1 < (i + 5); i1++){
        fprintf(stderr, "%llu: \"%s\"\n", i1, t.text[i1]);
      }
      exit(EXIT_FAILURE);
    }
    #endif

    //Get opcode from memonic
    u32 opcode = name_to_instr(t.text[i]);
    
    //remove memonic
    cstr tmp = cstrdup_stack(t.text[i]);
    if(strchr(tmp, ' '))
      tmp = strchr(tmp,  ' ') + 1; //Advance pointer to after the memonic and the spacebar after it
    else
      tmp = "0";
    
    u32 dest, a, b, c;
    
    //If the assembly does not need all of the arguments then sscanf will not set a,b,c so it is safe to use. 
    //All varibles are set to 0 which removes and possiblity that it will overrite any bits if set is used
    sscanf(tmp, "%u %u %u %u", &dest, &a, &b, &c); 
    if(logging){
      printf("pc: %llX:  %s %s\n", i * 4 + start_addr, opcode_str[opcode], tmp);
      //printf("pc: %llX:  %s %X, %X, %X\n", i * 4 + start_addr, opcode_str[opcode], a, b, c);
    }
    //When calling the function pointer, the function being called has no clue how many arguments are being passed in.
    //All argument registers are overwritten in this function and the called function has no way of telling if there is arguments set past it's
    //intended reach so this approach has no expliots avaible and condenses the code a lot.
    PUSH32(asm_opcode_ptr[opcode](opcode, dest, a, b, c));
  }
  
  vector_deconstruct(&tmp_asm.labels);
  vector_deconstruct(&tmp_asm.asm_global.globals);
  fu_free_text_file(t);
  return ret;
}

fu_BinFile INLINE assemble(char* __restrict__ file_path){
  fu_BinFile ret;
  fu_TextFile input = fu_load_text_file(file_path);
  ret = assemble_s(input);
  fu_free_text_file(input);
  return ret;
}
