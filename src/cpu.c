#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "cpu.h"
#include "opcode.h"
#include "types.h"
#include "bus.h"

//#include "gfxengine/gfxcommands.h"


#define INSTRUCTION_BITS 32

cpu_t cpu;
extern char logging;

INLINE void cpu_init(){
  cpu.pc = 0;
  cpu.lr = 0;
  cpu.cr.byte = 0;
  
  cpu.ctr = 0;

  //Clear BUS
  for(ptr_t i = 0; i < BUS_SIZE; i++){
    write8(i, 0);
  }

  //Clear registers
  for(u8 i = 0; i < 32; i++){
    cpu.r[i] = 0;
    cpu.f[i] = 0;
  }
  cpu.sp = STACK_SIZE;
}

void load_rom_data(const char* bytes, u32 size);//see bus.c. This line is just to keep the compiler happy ;)

INLINE void load_rom(const char* bytes, u32 size){
  cpu_init();
  init_opcodes();
  load_rom_data(bytes, size); //see bus.c
  cpu.pc = read32(ROM_START); //Get start addr
  
  //printf("START = %llu, %llu, %llu\n", cpu.pc + ROM_START, cpu.pc, ROM_START);
}

extern microcode_t opcode_ptr[OPCODE_LEN];
INLINE void step(){
  u32  opdat = read32(cpu.pc + ROM_START);
  opcode_ptr[GET_OP(opdat)](opdat);
  cpu.pc += 4;
}


#include "FileUtils.h"
extern fu_List execution_log;


void print_registers(){
  const char* NAMES[2] = {
    "R",
    "SP"
  };

  for(u32 i = 0; i < 32; i++){
    const char* word = NAMES[(i == 0)];
    char* log = NULL;

    if(logging){
      log = execution_log.text[execution_log.size - i - 1];
      if(log == NULL){
        log = " ";
      }
	    #ifndef WIN32
      if(i == 31){
        printf("\033[30;0m");
      }
	    #endif
    }
    //#endif
    char* tmp_str = alloca(500);
    //char* tmp_str2 = alloca(500);
    if(i > 9) {
      sprintf(tmp_str, "%s%u:\t%lX", word, i, cpu.ru[i]);
    }else{
      sprintf(tmp_str, "%s%u:\t%lX", word, i, cpu.ru[i]);
    }
    printf("%s\t", tmp_str);
    #if 0
    sprintf(tmp_str2, "FP%u:\t%f", i, cpu.f[i]);
    DrawString(0, i * 10, tmp_str);
    DrawString(200, i * 10, tmp_str2);
    sprintf(tmp_str, "PC: %X", (unsigned int)cpu.pc);
    sprintf(tmp_str2, "LR: %X",  (unsigned int)cpu.lr);
    DrawString(0, (32) * 10, tmp_str);
    DrawString(0, (33) * 10, tmp_str2);
    #endif
    if(logging) {
      puts(log);
	    #ifndef WIN32
      if(i == 31){
        printf("\033[30;0m");
      }
	    #endif
      
    } else {
      //(void)tmp_str2;
      putchar('\n');
    }
  }
  #ifndef WIN32
  if(logging){printf("\033[32;3mPC:\t%llX\nLR:\t%llX\n\033[30;0m", cpu.pc, cpu.lr);}
  #else
  if(logging){printf("PC:\t%llX\nLR:\t%llX\n", cpu.pc, cpu.lr);}
  #endif
}
//#endif

INLINE void print_cur(){
  //u32 opdat = read32(ROM_START + cpu.pc);
  //char* str = opcode_to_name(GET_OP(opdat));
  //puts(str);
  //#ifdef DEBUG
  print_registers();
  //#endif
  //free(str);
}

INLINE void jump(ptr_t addr){
  cpu.pc = addr - 4;
}

INLINE void jump_r(ptr_t addr){
  cpu.pc += addr - 4;
}

INLINE void jump_l(ptr_t addr){
  cpu.lr = cpu.pc;
  jump(addr);
}

INLINE void jump_l_r(ptr_t addr){
  cpu.lr = cpu.pc;
  jump_r(addr);
}
