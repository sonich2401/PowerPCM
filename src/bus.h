#pragma once

#include "types.h"
#include <stddef.h>

#if 0
#define SCREEN_W 640
#define SCREEN_H 320
#define BITDEPTH 8
#define FRAMEBUFFER_SIZE                                                       \
  ((unsigned long long)(SCREEN_W * SCREEN_H *                                  \
                        (((float)BITDEPTH) / ((float)8) + 0.4f) * 3))
#include "gfxengine/ppu.h" //OAM SIZE

#else

#define FRAMEBUFFER_SIZE 2
#define OAM_SIZE 0

#endif

#define STACK_SIZE 20000
#define ROM_SIZE 80000
#define EXT_DEVICE_SIZE 100


#if 1
#define BUS_SIZE 60000 + FRAMEBUFFER_SIZE + EXT_DEVICE_SIZE + STACK_SIZE + ROM_SIZE
#define RAM_SIZE                                                               \
  BUS_SIZE - OAM_SIZE - EXT_DEVICE_SIZE - ROM_SIZE - FRAMEBUFFER_SIZE

#else
#define RAM_SIZE 10000
#define BUS_SIZE FRAMEBUFFER_SIZE + EXT_DEVICE_SIZE + STACK_SIZE + ROM_SIZE + RAM_SIZE
#endif






#ifdef __cplusplus
extern "C" {
#endif



typedef union {
  struct {
    union {
      byte_t STACK[STACK_SIZE];
      byte_t RAM[RAM_SIZE];
    };
#if OAM_SIZE != 0
    //byte_t OAM[OAM_SIZE];
    OAM_t OAM;
    byte_t FRAMEBUFFER[FRAMEBUFFER_SIZE];
#else
    union {
      byte_t OAM[FRAMEBUFFER_SIZE];
      byte_t FRAMEBUFFER[FRAMEBUFFER_SIZE];
    };
#define DISABLE_OAM
#endif
    byte_t ROM[ROM_SIZE];
    byte_t EXT[EXT_DEVICE_SIZE];
  };

  byte_t DMA[BUS_SIZE];
} bus_t;


#define EXT_START (offsetof(bus_t, EXT))
#define EXT_END ((EXT_START + EXT_SIZE - 1) % BUS_SIZE)

#define ROM_START (offsetof(bus_t, ROM))
#define ROM_END ROM_START + ROM_SIZE - 1

#define FRAMEBUFFER_START (offsetof(bus_t, FRAMEBUFFER))
#define FRAMEBUFFER_END FRAMEBUFFER_START + FRAMEBUFFER_SIZE - 1

#define OAM_START (offsetof(bus_t, OAM))

#if OAM_SIZE != 0
#define  OAM_END OAM_START + OAM_SIZE - 1
#else
#define OAM_END RAM_SIZE
#define DISABLE_OAM
#endif

#define RAM_START 0
#define RAM_END OAM_START- 1

#ifdef __CPU_C__ //Dont let any other files other than cpu.c have access to this function
void load_rom_dat(const char* restrict bytes, u32 size);
#endif

#define BUS_FAST_RW




#ifndef BUS_FAST_RW

u8  read8(addr_t addr);
u16 read16(addr_t addr);
u32 read32(addr_t addr);
u64 read64(addr_t addr);

void write8(addr_t addr, u8 val);
void write16(addr_t addr, u16 val);
void write32(addr_t addr, u32 val);
void write64(addr_t addr, u64 val);

u8 syscall_scanf(u8 r1, u8 r2);

#else


extern bus_t bus;
#define BUS_WRITE_TEMPLATE(type, addr, val) *((type*)&bus.DMA[addr]) = val

#define BUS_READ_TEMPLATE(type, addr) *((type*)&bus.DMA[addr])

#define read8(addr) BUS_READ_TEMPLATE(u8, addr)
#define read16(addr) BUS_READ_TEMPLATE(u16, addr)
#define read32(addr) BUS_READ_TEMPLATE(u32, addr)
#define read64(addr) BUS_READ_TEMPLATE(u64, addr)

#define write8(addr, val) BUS_WRITE_TEMPLATE(u8, addr, val)
#define write16(addr, val) BUS_WRITE_TEMPLATE(u16, addr, val)
#define write32(addr, val) BUS_WRITE_TEMPLATE(u32, addr, val)
#define write64(addr, val) BUS_WRITE_TEMPLATE(u64, addr, val)

#define syscall_scanf(r1, r2) (scanf((const char*)&bus.DMA[cpu.ru[r1]], (const char*)&bus.DMA[cpu.ru[r2]]))


#endif

#ifdef __cplusplus
};
#endif
