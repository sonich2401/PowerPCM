#include "../opcode.h"
#include "FileUtils.h"
#include "types.h"

#include <malloc.h>


static INLINE u32 get_add(u8 dest, u8 src, u8 src2){
  u32 ret = 0;
  ret = SET_OP(ret, ADD);
  ret = SET_REG(ret, 1, dest);
  ret = SET_REG(ret, 2, src);
  ret = SET_REG(ret, 3, src2);

  return ret;
}

static INLINE u32 get_li(u8 dest, u32 imm){
  u32 ret = 0;
  ret = SET_OP(ret, LI);
  ret = SET_REG(ret, 1, dest);
  ret = SET_IMM(ret, imm);
  return ret;
}

static INLINE u32 get_addi(u8 dest, u8 a, u32 val){
  u32 ret = 0;
  
  ret = SET_OP(ret, ADDI);
  ret = SET_REG(ret, 1, dest);
  ret = SET_REG(ret, 2, a);
  ret = SET_ADD_IMM(ret, val);
  
  return ret;
}

static INLINE u32 get_subi(u8 dest, u8 a, u32 val){
  u32 ret = 0;
  
  ret = SET_OP(ret, SUBI);
  ret = SET_REG(ret, 1, dest);
  ret = SET_REG(ret, 2, a);
  ret = SET_ADD_IMM(ret, val);
  
  return ret;
}

static INLINE u32 get_sub(u8 dest, u8 a, u8 b){
  u32 ret = 0;
  
  ret = SET_OP(ret, SUB);
  ret = SET_REG(ret, 1, dest);
  ret = SET_REG(ret, 2, a);
  ret = SET_REG(ret, 3, b);
  
  return ret;
}

static INLINE u32 get_sw(u8 dest, u32 offset, u8 a){
  u32 ret = 0;
  
  ret = SET_OP(ret, SW);
  ret = SET_REG(ret, 1, dest);
  ret = SET_REG(ret, 2, a);
  ret = SET_INDEX_OFFSET(ret, offset);
  
  return ret;
}

static INLINE u32 get_cmpi(u8 dest, u32 val){
  u32 ret = 0;
  
  ret = SET_OP(ret, CMPI);
  ret = SET_REG(ret, 1, dest);
  ret = SET_IMM(ret, val);
  
  return ret;
}

static INLINE u32 get_bne(u8 dest, u32 val){
  u32 ret = 0;
  
  ret = SET_OP(ret, BNE);
  ret = SET_IMM_IMP(ret, val);
  
  return ret;
}

static INLINE u32 get_mr(u8 dest, u8 a){
  u32 ret = 0;
  
  ret = SET_OP(ret, MR);
  ret = SET_REG(ret, 1, dest);
  ret = SET_REG(ret, 2, a);
  
  return ret;
}

static INLINE u32 get_blr(){
  u32 ret = 0;
  return SET_OP(ret, BLR);
}

static INLINE u32 get_bl(u32 val){
  u32 ret = 0;
  ret = SET_OP(ret, BL);
  ret = SET_IMM_IMP(ret, val);
  
  return ret;
}

static INLINE u32 get_lw(u8 dest, u32 offset, u8 a){
  u32 ret = 0;
  
  ret = SET_OP(ret, LW);
  ret = SET_REG(ret, 1, dest);
  ret = SET_REG(ret, 2, a);
  ret = SET_INDEX_OFFSET(ret, offset);
  
  return ret;
}




/*
int main(){

}


int fib(register int max){
  register int r4 = 1;
  register int r5 = 0;
  do{
    r4 += r5;
    r5 -= r4;
  }while(--max != 0);
  return r4;
}


*/


static INLINE char* push32(char* __restrict__ buff, u32 opdat){
  *((u32*)buff) = opdat;
  buff += sizeof(u32);
  return buff;
}

u32 pc = 0;
#define PUSH32(dat) cursor = push32(cursor, dat); pc+=4

fu_BinFile get_program(){
  char* og_buff = (char*)malloc(1000);
  char* cursor = og_buff;
 
    //int main()
  PUSH32(sizeof(u32));
  PUSH32(get_li(3, 90)); // 4
  PUSH32(get_bl(pc + (2 * 4))); // 8
  PUSH32(END);
  
  //int fib(int max)
  PUSH32(get_subi(0, 0, 8)); //12
  PUSH32(get_sw(4, 4, 0)); //16
  PUSH32(get_sw(5, 0, 0)); //20
  //Init
  PUSH32(get_li(4, 1)); //24
  PUSH32(get_li(5, 0)); // 28
  
//RUN
  //do {
  PUSH32(get_add(4, 4, 5)); //32
  PUSH32(get_sub(5, 4, 5));
  PUSH32(get_subi(3, 3, 1)); //max--;
  //}while(max != 0);
  PUSH32(get_cmpi(3, 0));
  PUSH32(get_bne(4, pc - (4 * 4))); //4*4 = 4 instructions ago
  //max == 0
  //Exit
  PUSH32(get_mr(3, 4)); //return;
  
  //Cleanup
  PUSH32(get_lw(5, 0, 0));
  PUSH32(get_lw(4, 4, 0));
  PUSH32(get_addi(0, 0, 8));
  //Return
  PUSH32(get_blr());
  
  PUSH32(END);
  fu_BinFile ret;
  ret.bin = og_buff;
  ret.size = 1000;
  return ret;
}
