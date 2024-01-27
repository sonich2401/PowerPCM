#pragma once
#include "types.h"

typedef enum{
  CTR_NZERO_BI_ZERO    = 0b00000,
  CTR_ZERO_BI_ZERO     = 0b00010,
  CTR_BI_ZERO          = 0b00100,
  CTR_NZERO_BI_NZERO   = 0b01000,
  CTR_ZERO_BI_NZERO    = 0b01010,
  CTR_BI_NZERO         = 0b01100,
  CTR_NZERO            = 0b10000,
  CTR_ZERO             = 0b10010,
  CTR_BRANCH_ALL       = 0b10100,

  //Generic masks
  CTR_DEC_CTR_NO_BI    = 0b10000,
  CTR_ONLY_CHECK_CI    = 0b00100
}CTR_Branch_t;


typedef union{
  struct {
  char N : 1;
  char L : 1; //TODO: Change to greater than
  char E : 1;
  char SO : 1;
  char I : 1;
  char ERR : 1;
  };
  char byte;
}CR_t;


typedef struct{
  ptr_t pc;
  ptr_t lr;

  CR_t cr;
  
  unsigned long long ctr;
  

  union {
    long r[32];
    unsigned long ru[32];

    struct{
      ptr_t sp;
    };
  };

  float f[32];

}cpu_t;


extern cpu_t cpu;



void cpu_init();
void load_rom(const char* bytes, u32 size);

void step();
void print_cur();

void jump(ptr_t addr);
void jump_r(ptr_t addr);
void jump_l(ptr_t addr);
void jump_l_r(ptr_t addr);
