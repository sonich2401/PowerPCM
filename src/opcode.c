#include "opcode.h"
#include "bus.h"
#include "cpu.h"
#include "types.h"

#include "FileUtils.h"

#include <stdio.h>


fu_List execution_log;


const char* opcode_str[OPCODE_LEN] = {
	"NOP",
	"LI",
	"LB",
	"LHW",
	"LW",
	"SB",
	"SHW",
	"SW",
	"AND",
	"ANDI",
	"OR",
	"ORI",
	"XOR",
	"XORI",
	"ADD",
	"ADDI",
	"SUB",
	"SUBI",
	"MUL",
	"MULI",
	"DIV",
	"DIVI",
	
	"MFLR",
	"MTLR",
	"MFCTR",
	"MTCTR",
	
	"MR",
	"BNE",
	"BEQ",
	"BGT",
	"BGE",
	"BLT",
	"BLE",
	
	"BDNZ",
	"BDZ",
	"BDCTRL",
	
	"CMP",
	"CMPI",
	"BLR",
	"BL",
	"B",
	"SL",
	"SLI",
	"SR",
	"SRI",
	
	"SC",
	"END"
};

#include <stdlib.h>


char* push_string(){
	char* ret = (char*)malloc(1500);

	if(execution_log.text[execution_log.size - 1] != NULL){
		free(execution_log.text[execution_log.size - 1]);
	}
	
	for(unsigned int i = execution_log.size - 1; i > 0; i--){
		execution_log.text[i] = execution_log.text[i - 1];
	}
	
	execution_log.text[0] = ret;
	
	return ret;
}

extern char logging;

#include <stdlib.h>
#include <string.h>
//#ifdef LOG
	#if 0
		#define dbg_printf(...) printf(__VA_ARGS__)
	#else
		#define dbg_printf(...) if(logging){char* ___tmp___ = push_string();\
		sprintf(___tmp___, __VA_ARGS__); \
		___tmp___= strrchr(___tmp___, '\n');\
		if(___tmp___ != NULL){ \
			*___tmp___ = 0;\
		}}\
		//printf(__VA_ARGS__);
		
	#endif
//#else
//	#if 1
//		char do_nothing[1000];
//		#define dbg_printf(...) sprintf(do_nothing, __VA_ARGS__)
//	#else
//		#define dbg_printf(...) printf(__VA_ARGS__)
//	#endif
//#endif


typedef unsigned long long arg_t;

#define TYPE_0 ((void)opdat); \
dbg_printf("%s\n", opcode_str[GET_OP(opdat)]);

#define TYPE_A ((void)opdat); \
u8 dest = GET_REG(opdat, 1); \
u8 a = GET_REG(opdat, 2); \
u8 b = GET_REG(opdat, 3); \
dbg_printf("%s R%u, R%u, R%u\n", opcode_str[GET_OP(opdat)], dest, a, b);

#define TYPE_B ((void)opdat); \
u8 dest = GET_REG(opdat, 1); \
u8 a = GET_REG(opdat, 2); \
u32 b = GET_ADD_IMM(opdat); \
dbg_printf("%s R%u, R%u, %u\n", opcode_str[GET_OP(opdat)], dest, a, b);

#define TYPE_C ((void)opdat); \
  u8 dest = GET_REG(opdat, 1); \
  u8 a = GET_REG(opdat, 2); \
  u32 offset = GET_INDEX_OFFSET(opdat);\
  dbg_printf("%s R%u, %u(R%u)\n", opcode_str[GET_OP(opdat)], dest, offset, a);
  
#define TYPE_D ((void)opdat); \
u8 dest = GET_REG(opdat, 1); \
u32 imm = GET_IMM(opdat); \
dbg_printf("%s R%u, %u\n", opcode_str[GET_OP(opdat)], dest, imm);

#define TYPE_E ((void)opdat); \
u8 dest = GET_REG(opdat, 1); \
dbg_printf("%s R%u\n", opcode_str[GET_OP(opdat)], dest);

#define TYPE_F ((void)opdat); \
u32 imm = GET_IMM_IMP(opdat); \
dbg_printf("%s %u\n", opcode_str[GET_OP(opdat)], imm);

void func_nop(){
  puts("NOP");
  return;
}


//MULTIPLY FUNCTIONS
#define mul_template(dest, a, b) (cpu.r[dest] = cpu.r[a] * b)

void func_mul(u32 opdat){
  TYPE_A
  mul_template(dest, a, cpu.r[b]);
}

void func_muli(u32 opdat){
  TYPE_B
  mul_template(dest, a, b);
}

//DIVIDE FUNCTION
#define div_template(dest, a, b) (cpu.r[dest] = cpu.r[a] / b)

void func_div(u32 opdat){
  TYPE_A
  div_template(dest, a, cpu.r[b]);
}

void func_divi(u32 opdat){
  TYPE_B
  div_template(dest, a, b);
}


//ADD FUNCTIONS
#define add_template(dest, a, b) (cpu.r[dest] = cpu.r[a] + b)

void func_add_reg(u32 opdat){
  TYPE_A
  add_template(dest, a, cpu.r[b]);
}

void func_add_imm(u32 opdat){
  TYPE_B
  add_template(dest, a, b);
}

//Subtract functions
#define sub_template(dest, a, b) (cpu.r[dest] = cpu.r[a] - b)

void func_sub(u32 opdat){
	TYPE_A
	sub_template(dest, a, cpu.ru[b]);
}

void func_subi(u32 opdat){
	TYPE_B
	sub_template(dest, a, b);
}



//LOAD FUNCTIONS

void func_li(u32 opdat){
  TYPE_D
  cpu.ru[dest] = imm;
}

void func_load_byte(u32 opdat){
  TYPE_C
  cpu.ru[dest] = read8(cpu.ru[a] + CONV_S32(offset));
}

void func_load_halfword(u32 opdat){
  TYPE_C
  cpu.ru[dest] = read16(cpu.ru[a] + CONV_S32(offset));
}

void func_load_word(u32 opdat){
  TYPE_C
  cpu.ru[dest] = read32(cpu.ru[a] + CONV_S32(offset));
}

//STORE FUNCTIONS
void func_sb(u32 opdat){
  TYPE_C
  write8(cpu.ru[a] + CONV_S32(offset), cpu.ru[dest] & 0xFF);
}

void func_shw(u32 opdat){
  TYPE_C
  write16( cpu.ru[a] + CONV_S32(offset), cpu.ru[dest] & 0xFFFF);
}

void func_sw(u32 opdat){
  TYPE_C
  write32( cpu.ru[a] + CONV_S32(offset), cpu.ru[dest]);
}



//COMPARE FUNCTIONS

static inline void cmp_template(u32 val_a, u32 val_b){
	//printf("%u, %u\n", val_a, val_b);
	//getc(stdin);
	cpu.cr.E = (val_a == val_b);
	cpu.cr.L = (val_a < val_b);
	cpu.cr.N = ((((s32)val_a) - ((s32)val_b)) < 0 );
}

static void func_cmp(u32 opdat){
	TYPE_A
	(void)b;
	
	cmp_template(cpu.ru[dest], cpu.ru[a]);
}

static void func_cmpi(u32 opdat){
	TYPE_D
	
	cmp_template(cpu.ru[dest], imm);
}

//BRANCHING INSTRUCTIONS
static void func_b(u32 opdat){
	TYPE_F
	jump_l(imm);
}

static void func_bl(u32 opdat){
	TYPE_F
	jump_l(imm);
}

static void func_beq(u32 opdat){
	TYPE_F
	if(cpu.cr.E){
		jump(imm);
	}
}

static void func_bne(u32 opdat){
	TYPE_F
	if(!cpu.cr.E){
		jump(imm);
	}
}

static void func_blt(u32 opdat){
	TYPE_F
	if(cpu.cr.L){
		jump(imm);
	}
}

static void func_ble(u32 opdat){
	TYPE_F
	if(cpu.cr.E || cpu.cr.L){
		jump(imm);
	}
}

static void func_bgt(u32 opdat){
	TYPE_F
	if(!cpu.cr.L){
		jump(imm);
	}
}

static void func_bge(u32 opdat){
	TYPE_F
	if(cpu.cr.E || !cpu.cr.L){
		jump(imm);
	}
}

static void func_bdz(u32 opdat){
	TYPE_F
	
	if(!(--cpu.ctr)){
		jump(imm);
	}
}

static void func_bdnz(u32 opdat){
	TYPE_F
	
	if((--cpu.ctr)){
		jump(imm);
	}
}

static void func_bdctrl(u32 opdat){
	TYPE_0
	
	jump_l(cpu.ctr);
}

static void func_blr(u32 opdat){
	TYPE_0
	cpu.pc = cpu.lr;
}


//SHIFT FUNCTIONS
static inline void shift_left_template(u8 dest, u8 a, u32 b){
	cpu.ru[dest] = cpu.ru[a] << b;
}

static void func_sl(u32 opdat){
	TYPE_A
	shift_left_template(dest, a, cpu.ru[b]);
}

static void func_sli(u32 opdat){
	TYPE_B
	shift_left_template(dest, a, b);
}

static inline void shift_right_template(u8 dest, u8 a, u32 b){
	cpu.ru[dest] = cpu.ru[a] >> b;
}

static void func_sr(u32 opdat){
	TYPE_A
	shift_right_template(dest, a, cpu.ru[b]);
}

static void func_sri(u32 opdat){
	TYPE_B
	shift_right_template(dest, a, b);
}


//BITWISE LOGIC
static inline void and_template(u8 dest, u8 a, u32 b){
	cpu.ru[dest] = cpu.ru[a] & b;
}

static void func_and(u32 opdat){
	TYPE_A
	and_template(dest, a, cpu.ru[b]);
}

static void func_andi(u32 opdat){
	TYPE_B
	and_template(dest, a, b);
}

static inline void or_template(u8 dest, u8 a, u32 b){
	cpu.ru[dest] = cpu.ru[a] | b;
}

static void func_or(u32 opdat){
	TYPE_A
	or_template(dest, a, cpu.ru[b]);
}

static void func_ori(u32 opdat){
	TYPE_B
	or_template(dest, a, b);
}

static inline void xor_template(u8 dest, u8 a, u32 b){
	cpu.ru[dest] = cpu.ru[a] ^ b;
}

static void func_xor(u32 opdat){
	TYPE_A
	xor_template(dest, a, cpu.ru[b]);
}

static void func_xori(u32 opdat){
	TYPE_B
	xor_template(dest, a, b);
}



//MISC. FUNCTIONS
static void func_mr(u32 opdat){
	TYPE_A
	(void)b;
	cpu.ru[dest] = cpu.ru[a];
}


//SYSTEM FUNCTIONS
#include <stdint.h>
static void func_syscall(u32 opdat){
	TYPE_F
	
	int err = 0;

	switch(imm){
		case PUT:
			//printf("%u, %X, \"%c\"\n", (unsigned)cpu.ru[3], (unsigned)cpu.ru[3], (char)cpu.ru[3]);
			putc(cpu.ru[3], stdout);
			return;
		break;
		case SCAN:
			err = syscall_scanf(3, 4);
			if(err != 0){
				cpu.cr.ERR = 1;
			}
		break;
		
		case RAND:
			cpu.ru[3] = rand() % cpu.ru[4];
		break;
		default:
			cpu.cr.ERR = 1;
		break;
	}
}

static void func_mtlr(u32 opdat){
	TYPE_E
	u32 val = cpu.ru[dest];
	cpu.lr = val;
}

static void func_mflr(u32 opdat){
	TYPE_E
	cpu.ru[dest] = cpu.lr;
}

static void func_mtctr(u32 opdat){
	TYPE_E
	cpu.ctr = cpu.ru[dest];
}

static void func_mfctr(u32 opdat){
	TYPE_E
	cpu.ru[dest] = cpu.ctr;
}


#include <stdlib.h>
static void func_end(u32 opdat){
	TYPE_F
	//extern char stop_exe;
	//stop_exe = 1;
	exit(imm);
}

microcode_t opcode_ptr[OPCODE_LEN];


#include <string.h>
microcode_t decode_opcode_name(const char* restrict name){
  for(unsigned char i = 0; i < OPCODE_LEN; i++){
    if(strcmp(name, opcode_str[i]) == 0){
      return opcode_ptr[i];
    }
  }
  
  return func_nop;
}



inline char* opcode_to_name(unsigned char opcode){
 return strdup(opcode_str[opcode]);
}


inline microcode_t decode_bin(const char* bytes){
       unsigned char opcode = GET_OP((*(unsigned char*)bytes));
	//if(OPCODE_LEN <= opcode){
  //            puts("WARNING: Invalid opcode!");
  //            abort();
//		return func_nop;
 //      }
       microcode_t ret = opcode_ptr[opcode];
  //     if(ret == NULL){
 //             printf("WARNING: Function pointer at %X not set!\n", opcode);
  //            return func_nop;
  //     }
       return ret;
}


#define DEF_OP(OP, func) opcode_ptr[(OP)] = (func)
void init_opcodes(){
	//#ifdef LOG
	execution_log.size = 32;
	execution_log.text = (char**)malloc(sizeof(char*) * execution_log.size);
	//#endif

	//Set all functions to nop
	for(u8 i = 0; i < OPCODE_LEN; i++){
		DEF_OP(i, func_nop);
		//#ifdef LOG
		execution_log.text[i % execution_log.size] = NULL;
		//#endif
	}
	
	DEF_OP(ADD, func_add_reg);
	DEF_OP(ADDI, func_add_imm);
	
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
	
	DEF_OP(BL, func_bl);
	DEF_OP(B, func_b);
	DEF_OP(BLR, func_blr);
	DEF_OP(BEQ, func_beq);
	DEF_OP(BNE, func_bne);
	DEF_OP(BLT, func_blt);
	DEF_OP(BLE, func_ble);
	DEF_OP(BGT, func_bgt);
	DEF_OP(BGE, func_bge);
	
	DEF_OP(BDZ, func_bdz);
	DEF_OP(BDNZ, func_bdnz);
	DEF_OP(BDCTRL, func_bdctrl);
	
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
	
	DEF_OP(MR, func_mr);
	DEF_OP(MFLR, func_mflr);
	DEF_OP(MTLR, func_mtlr);
	DEF_OP(MFCTR, func_mfctr);
	DEF_OP(MTCTR, func_mtctr);
	DEF_OP(SC, func_syscall);
	DEF_OP(END, func_end);
	
	for(unsigned int i = 1; i < OPCODE_LEN; i++){
		if(opcode_ptr[i] == func_nop){
			printf("UNIMP = %s\n", opcode_str[i]);
		}
	}
}
