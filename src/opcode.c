#include "opcode.h"
#include "bus.h"
#include "cpu.h"
#include "types.h"

#include "FileUtils.h"
#include "vector.h"

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
	"NOT",
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
	"BCTRL",
	
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


const ADDR_MODE_t opcode_modes[OPCODE_LEN] = {
	ADDR_0,
	ADDR_D,
	ADDR_C, //LB
	ADDR_C,
	ADDR_C,
	ADDR_C, //SB
	ADDR_C,
	ADDR_C,
	//AND
	ADDR_A,
	ADDR_B,
	//OR
	ADDR_A,
	ADDR_B,
	//XOR
	ADDR_A,
	ADDR_B,
	//NOT
	ADDR_F,
	//ADD
	ADDR_A,
	ADDR_B,
	//SUB
	ADDR_A,
	ADDR_B,
	//MUL
	ADDR_A,
	ADDR_B,
	//DIV
	ADDR_A,
	ADDR_B,
	//MFLR
	ADDR_E,
	ADDR_E,
	//MTCTR
	ADDR_E,
	ADDR_E,
	//MR
	ADDR_A,
	//BRANCHING
	ADDR_F, //BNE
	ADDR_F, //BEQ
	ADDR_F, //BGT
	ADDR_F, //BGE
	ADDR_F, //BLT
	ADDR_F, //BLE
	//BRANCH CTRL
	ADDR_F, //BDNZ
	ADDR_F, //BDZ
	ADDR_F, //BCTRL
	//COMPARE
	ADDR_A,
	ADDR_D,
	//BRANCHING AGAIN
	ADDR_0, //BLR
	ADDR_F, //BL
	ADDR_F, //B
	//SHIFT
	ADDR_A, //SL
	ADDR_B, //SLI
	ADDR_A, //SR
	ADDR_B, //SRI

	//SYSTEM
	ADDR_F, //SC
	ADDR_F, //END
};

#include <stdlib.h>


static char* push_string(){
	char* ret = (char*)malloc(40);

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
#if 1
	#if 0
		#define dbg_printf(...) printf(__VA_ARGS__)
	#else
		#define dbg_printf(...) if(logging){char* ___tmp___ = push_string();\
		___tmp___[sprintf(___tmp___, __VA_ARGS__) - 1] = 0; }
		
	#endif
#else
	#if 0
		char do_nothing[1000];
		#define dbg_printf(...) sprintf(do_nothing, __VA_ARGS__)
	#else
		#define dbg_printf(...)
	#endif
#endif


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

static void func_nop(){
}


//MULTIPLY FUNCTIONS
#define mul_template(dest, a, b) (cpu.r[dest] = cpu.r[a] * b)

static void func_mul(u32 opdat){
  TYPE_A
  mul_template(dest, a, cpu.r[b]);
}

static void func_muli(u32 opdat){
  TYPE_B
  mul_template(dest, a, b);
}

//DIVIDE FUNCTION
#define div_template(dest, a, b) (cpu.r[dest] = cpu.r[a] / b)

static void func_div(u32 opdat){
  TYPE_A
  div_template(dest, a, cpu.r[b]);
}

static void func_divi(u32 opdat){
  TYPE_B
  div_template(dest, a, b);
}


//ADD FUNCTIONS
#define add_template(dest, a, b) (cpu.r[dest] = cpu.r[a] + b)

static void func_add_reg(u32 opdat){
  TYPE_A
  add_template(dest, a, cpu.r[b]);
}

static void func_add_imm(u32 opdat){
  TYPE_B
  add_template(dest, a, b);
}

//Subtract functions
#define sub_template(dest, a, b) (cpu.r[dest] = cpu.r[a] - b)

static void func_sub(u32 opdat){
	TYPE_A
	sub_template(dest, a, cpu.ru[b]);
}

static void func_subi(u32 opdat){
	TYPE_B
	sub_template(dest, a, b);
}



//LOAD FUNCTIONS

static void func_li(u32 opdat){
  TYPE_D
  cpu.ru[dest] = imm;
}

static void func_load_byte(u32 opdat){
  TYPE_C
  cpu.ru[dest] = read8(cpu.ru[a] + CONV_S32(offset));
}

static void func_load_halfword(u32 opdat){
  TYPE_C
  cpu.ru[dest] = read16(cpu.ru[a] + CONV_S32(offset));
}

static void func_load_word(u32 opdat){
  TYPE_C
  cpu.ru[dest] = read32(cpu.ru[a] + CONV_S32(offset));
}

//STORE FUNCTIONS
static void func_sb(u32 opdat){
  TYPE_C
  write8(cpu.ru[a] + CONV_S32(offset), cpu.ru[dest] & 0xFF);
}

static void func_shw(u32 opdat){
  TYPE_C
  write16( cpu.ru[a] + CONV_S32(offset), cpu.ru[dest] & 0xFFFF);
} 

static void func_sw(u32 opdat){
  TYPE_C
  write32( cpu.ru[a] + CONV_S32(offset), cpu.ru[dest]);
}



//COMPARE FUNCTIONS

static INLINE void cmp_template(u32 val_a, u32 val_b){
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
static INLINE void shift_left_template(u8 dest, u8 a, u32 b){
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

static INLINE void shift_right_template(u8 dest, u8 a, u32 b){
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
static INLINE void and_template(u8 dest, u8 a, u32 b){
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

static INLINE void or_template(u8 dest, u8 a, u32 b){
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

static INLINE void xor_template(u8 dest, u8 a, u32 b){
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

static void func_not(u32 opdat){
	TYPE_F
	cpu.ru[imm] = ~cpu.ru[imm];
}



//MISC. FUNCTIONS
static void func_mr(u32 opdat){
	TYPE_A
	(void)b;
	cpu.ru[dest] = cpu.ru[a];
}


//SYSTEM FUNCTIONS
//https://winjia.medium.com/linux-getch-for-unix-c2c829721a30
#ifndef WIN32
#include <termios.h>
#include <unistd.h>

static int getch(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

static int getche(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}
#else
#include <conio.h>
#endif

#include "gfx/ppu.h"
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
		case GETCH:
			if(cpu.ru[4] != 0){
				cpu.ru[3] = getche();
			}else{
				cpu.ru[3] = getch();
			}
		break;
		case GFX:
			ppu_process_ext_device_command();
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
microcode_t INLINE decode_opcode_name(const char* restrict name){
  for(unsigned char i = 0; i < OPCODE_LEN; i++){
    if(strcmp(name, opcode_str[i]) == 0){
      return opcode_ptr[i];
    }
  }
  
  return func_nop;
}



INLINE char* opcode_to_name(unsigned char opcode){
 return strdup(opcode_str[opcode]);
}


char* encode_data_block(fu_hexarray ptr, fu_index size){
	char* text = NULL; 
	(void)asprintf(&text, "DATA ");
	for(fu_index i = 0; i < size; i++){
		char* str_backup = text;
		(void)asprintf(&text, "%s 0x%X ", str_backup, *(u8*)ptr);
		free(str_backup);
		ptr++;
	}
	return text;
}

void simplify_memory_vector(vector* v){
	for(fu_index i = 0; i < vector_size(v); i++){
		fu_BinFile* region = (fu_BinFile*)vector_index(v, i);
		for(fu_index j = 0; j < vector_size(v); j++){
			if(i == j) continue;
			fu_BinFile region1 = *(fu_BinFile*)vector_index(v, j);
			if(region->bin <= region1.bin && region->bin + region->size >= region1.bin){
				fu_BinFile new_bin;
				new_bin.bin = region->bin;
				new_bin.size = (region1.bin + region1.size) - region->bin;

				//Overrite the old region block with the expanded version
				*region = new_bin;

				//remove element j (aka region1)
				vector_erase(v, j);

				//Move cursor back so when the ++ happens it will stay in the current cursor position
				j--;
			}
		}
	}
}

void sort_mem_vector(vector* v){
	for(fu_index i = 1; i < vector_size(v); i++){
		fu_BinFile b1 = *(fu_BinFile*)vector_index(v, i - 1);
		fu_BinFile b2 = *(fu_BinFile*)vector_index(v, i);

		if(b2.bin < b1.bin){
			*(fu_BinFile*)vector_index(v, i - 1) = b2;
			*(fu_BinFile*)vector_index(v, i) = b1;
			i = 1; //Restart checking
		}
	}
}

bool does_memory_block_exist(fu_hexarray ptr, vector* v){
	for(fu_index i = 0; i < vector_size(v); i++){
		fu_BinFile region = *(fu_BinFile*)vector_index(v, i);
		if(region.bin == ptr) return true;
	}
	return false;
}

void find_branches(fu_hexarray ptr, vector* v, fu_hexarray bin_start, fu_hexarray bin_end){
	if(does_memory_block_exist(ptr, v)){
		return;
	}
	fu_BinFile block;
	block.bin = ptr;
	block.size = 0;
	while(ptr < bin_end){
		opcode_t opdat = *(opcode_t*)ptr;
		ADDR_MODE_t addr_mode = opcode_modes[GET_OP(opdat)];

		if(addr_mode == ADDR_F){ //ADDR_F is almost always used for branching
			find_branches(bin_start + GET_IMM_IMP(opdat), v, bin_start, bin_end);
		}
		if(GET_OP(opdat) == B || GET_OP(opdat) == BL || GET_OP(opdat) == BLR){
			break;
		}

		ptr += sizeof(opcode_t);
		block.size += sizeof(opcode_t);
	}
	vector_push_back(v, &block);
	simplify_memory_vector(v);
}

/*
for(fu_index i = 1; i < ret.size; i++){
		opcode_t opdat = *(opcode_t*)bin.bin;

		ADDR_MODE_t addr_mode = opcode_modes[GET_OP(opdat)];
		char* formatted_string = NULL;

		//Vars for switch
		u8 dest = 0;
		u8 a = 0;
		u8 b = 0;
		u32 offset = 0;
		u32 imm = 0;

		switch(addr_mode){
			case ADDR_0:
				asprintf(&formatted_string, "%s", opcode_str[GET_OP(opdat)]);
			break;
			case ADDR_A:
				dest = GET_REG(opdat, 1);
				a = GET_REG(opdat, 2);
				b = GET_REG(opdat, 3);
				asprintf(&formatted_string, "%s R%u, R%u, R%u", opcode_str[GET_OP(opdat)], dest, a, b);
			break;
			case ADDR_B:
				dest = GET_REG(opdat, 1);
				a = GET_REG(opdat, 2);
				b = GET_ADD_IMM(opdat);
				asprintf(&formatted_string, "%s R%u, R%u, %u", opcode_str[GET_OP(opdat)], dest, a, b);
			break;
			case ADDR_C:
				dest = GET_REG(opdat, 1);
  				a = GET_REG(opdat, 2);
  				offset = GET_INDEX_OFFSET(opdat);
  				asprintf(&formatted_string, "%s R%u, %u(R%u)", opcode_str[GET_OP(opdat)], dest, offset, a);
			break;
			case ADDR_D:
				dest = GET_REG(opdat, 1);
				imm = GET_IMM(opdat);
				asprintf(&formatted_string, "%s R%u, %u", opcode_str[GET_OP(opdat)], dest, imm);
			break;
			case ADDR_E:
				dest = GET_REG(opdat, 1);
				asprintf(&formatted_string, "%s R%u", opcode_str[GET_OP(opdat)], dest);
			break;
			case ADDR_F:
				imm = GET_IMM_IMP(opdat);
				asprintf(&formatted_string, "%s %u", opcode_str[GET_OP(opdat)], imm);
			break;

			default:
				fprintf(stderr, "WARN: Unknown opcode type of %c!\n", (unsigned char)(opcode_modes[GET_OP(opdat)] + 1 + 'A'));
			break;
		}

		ret.text[i] = formatted_string;
		bin.bin += sizeof(opcode_t); //Move to next opcode
	}
*/


void free_bin_file_vector_version(fu_BinFile* b){
	fu_free_bin_file(*b);
}

fu_TextFile decode_bin(fu_BinFile bin){     
	fu_TextFile ret = fu_alloc_text_file((bin.size / sizeof(opcode_t)) + 1); //This garentees at least enough lines
	vector mem_map = vector_create(sizeof(fu_BinFile), free_bin_file_vector_version);
	u32 start_pc_vector = *(u32*)bin.bin;
	find_branches(bin.bin + start_pc_vector, &mem_map, bin.bin, bin.bin + bin.size);
	sort_mem_vector(&mem_map);

	for(fu_index i = 1; i < vector_size(&mem_map); i++){
		fu_BinFile b1 = *(fu_BinFile*)vector_index(&mem_map, i - 1);
		fu_BinFile b2 = *(fu_BinFile*)vector_index(&mem_map, i);
		fu_hexarray start = b1.bin + b1.size;
		fu_hexarray end = b2.bin;
		encode_data_block(start, end-start);
	}
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
	DEF_OP(BCTRL, func_bdctrl);
	
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
	
	for(unsigned int i = 1; i < OPCODE_LEN; i++){
		if(opcode_ptr[i] == func_nop){
			printf("UNIMP = %s\n", opcode_str[i]);
		}
	}
}
