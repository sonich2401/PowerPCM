#pragma once
#include "types.h"

#define OPCODE_LEN 47

typedef unsigned int opcode_t;

typedef void(*microcode_t)(u32);


#define OP_BITMASK 0b111111
#define OP_SIZE 6

#define GET_OP(dat) ((dat) & OP_BITMASK)
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



typedef enum {
	NOP,
	LI,
	LB,
	LHW,
	LW,
	SB,
	SHW,
	SW,
	AND,
	ANDI,
	OR,
	ORI,
	XOR,
	XORI,
	ADD,
	ADDI,
	SUB,
	SUBI,
	MUL,
	MULI,
	DIV,
	DIVI,
	MFLR,
	MTLR,
	MFCTR,
	MTCTR,
	MR,
	
	BNE,
	BEQ,
	BGT,
	BGE,
	BLT,
	BLE,
	
	BDNZ,
	BDZ,
	BCTRL,
	
	CMP,
	CMPI,
	BLR,
	BL,
	B,
	SL,
	SLI,
	SR,
	SRI,
	
	SC,
	END
}opcode_enum;


typedef enum{
	PUT,
	SCAN,
	RAND,
	GETCH,
	GFX
}SYSCALL_T;



microcode_t decode_opcode_name(const char* __restrict__ name);
char* opcode_to_name(unsigned char opcode);
microcode_t decode_bin(const char* bytes);
void init_opcodes();
