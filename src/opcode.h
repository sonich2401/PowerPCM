#pragma once

#include "types.h"
#include "FileUtils.h"

//PPC ISA stuff
#include "isa/ppc_isa.h"
#include "isa/forms.h"
//


#define OPCODE_LEN 0b111111


#define OP_BITMASK 0b111111
#define OP_SIZE 6

#define GET_OP(dat) ((((GENERIC_FORM_t*)&dat))->opcode)
#define SET_OP(dat, opcode) (((dat) &(~OP_BITMASK)) | ((opcode) & OP_BITMASK))

#define GET_REG(dat, reg_num) (((dat) >> (OP_SIZE + (5 * (reg_num - 1)))) & 0b11111)
#define SET_REG(dat, reg_num, val) (((dat) &(~(0b11111 << (OP_SIZE + (5 * (reg_num - 1)))))) | ((val) << (OP_SIZE + (5 * (reg_num - 1)))))

#define GET_IMM(dat) ((dat) >> (5 + OP_SIZE))
#define SET_IMM(dat, val) (((dat) & 0b11111111111) | ((val) << (5 + OP_SIZE)))

#define GET_IMM_IMP(dat) ((dat) >> OP_SIZE)
#define SET_IMM_IMP(dat, val) (((dat) & OP_BITMASK) | ((val) << OP_SIZE))

#define GET_ADD_IMM(dat) ((dat) >> ((5 * 2) + OP_SIZE))
#define SET_ADD_IMM(dat, val) (((dat) & 0b1111111111111111) | ((val) << ((5 * 2) + OP_SIZE)))

#define GET_INDEX_OFFSET(dat) ((dat) >> 16)
#define SET_INDEX_OFFSET(dat, val) ((0b11111111111111111 & (dat)) | ((val) << 16))

typedef u32 opcode_t;
typedef void(*microcode_t)(u32);

typedef enum{
	PUT,
	SCAN,
	RAND,
	GETCH,
	GFX
}SYSCALL_T;


extern const char* OPCODE_NAMES[OPCODE_LEN];


microcode_t decode_opcode_bytes(u32 bytes);
microcode_t decode_opcode_name(const char* __restrict__ name);
char* opcode_to_name(unsigned char opcode);
fu_TextFile decode_bin(fu_BinFile bin);
void init_opcodes();