#pragma once
#include "types.h"

typedef union{
  struct {
  char N : 1;
  char L : 1;
  char I : 1;
  char E : 1;
  char ERR : 1;
  };
  char byte;
}CR_t;


typedef struct{
  ptr_t pc;
  ptr_t lr;

  CR_t cr;
  

  union {
    long long r[32];
    unsigned long long ru[32];

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
