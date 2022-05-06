#include "bus.h"
#include "cpu.h"
#ifdef BUS_FAST_RW
static bus_t bus;
#else
bus_t bus;
#endif


#ifndef BUS_FAST_RW
#include <stdlib.h>
#include <stdio.h>

#ifndef BUS_NO_OOB_CHECK
void oob_check(addr_t addr){
  return;
  if(addr >= BUS_SIZE){
    fprintf(stderr, "ERR bus.c: addr = %X and is bigger than the max %X by the amount %X\n", addr, BUS_SIZE, addr - BUS_SIZE);
    abort();
  }
}
#else
#define oob_check(addr) ((void)addr)
#endif

#define READ_TEMPLATE(type) oob_check(addr); return (*(type*)&bus.DMA[addr])

#define WRITE_TEMPLATE(type) oob_check(addr); (*(type*)&bus.DMA[addr]) = val

u8  read8(addr_t addr){
  READ_TEMPLATE(u8);
}
u16 read16(addr_t addr){
  READ_TEMPLATE(u16);
}
u32 read32(addr_t addr){
  READ_TEMPLATE(u32);
}
u64 read64(addr_t addr){
  READ_TEMPLATE(u64);
}

void write8(addr_t addr, u8 val){
  WRITE_TEMPLATE(u8);
}
void write16(addr_t addr, u16 val){
  WRITE_TEMPLATE(u16);
}
void write32(addr_t addr, u32 val){
  WRITE_TEMPLATE(u32);
}
void write64(addr_t addr, u64 val){
  WRITE_TEMPLATE(u64);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void load_rom_data(const char* restrict bytes, u32 size){
  //memcpy(&bus.ROM[0], bytes, size);
  
  //Memcpy
  for(u32 i = 0; i < size; i++){
    bus.ROM[i] = bytes[i];
  }
}


#endif
