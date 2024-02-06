#include "opcode.h"
#include "bus.h"
#include "cpu.h"
#include "types.h"

#include "FileUtils.h"
#include "vector.h"


#include <stdio.h>

const char* OPCODE_NAMES[OPCODE_LEN];
fu_List execution_log;

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


/*
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
*/

#define TYPE_I I_FORM_t form = 		*((I_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);//dbg_printf("%s %u\n", OPCODE_NAMES[form.opcode], signex(form.LI, 64) + (cpu.pc * !form.AA));
#define TYPE_D D_FORM_t form = 		*((D_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);//dbg_printf("%s R%u R%u %li\n", OPCODE_NAMES[form.opcode], form.RT, form.RA, signex(form.SI, 64));
#define TYPE_DS DS_FORM_t form =	*((DS_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_B B_FORM_t form =		*((B_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_SC SC_FORM_t form =	*((SC_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_X X_FORM_t form =		*((X_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_XL XL_FORM_t form =	*((XL_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_XFX XFX_FORM_t form =	*((XFX_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_XFL XFL_FORM_t form =	*((XFL_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_XS XS_FORM_t form =	*((XS_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_XO XO_FORM_t form =	*((XO_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_A A_FORM_t form =		*((A_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_M M_FORM_t form =		*((M_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_MD MD_FORM_t form =	*((MD_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);
#define TYPE_MDS MDS_FORM_t form =	*((MDS_FORM_t*)(&opdat));((void)form);dbg_printf("%s %X\n", OPCODE_NAMES[form.opcode], opdat);

#define IMP_ME_PLEASE fprintf(stderr, "FATAL: Instruction %X '%s' not implemented yet! Opdat was %X\n", GET_OP(opdat), __PRETTY_FUNCTION__, opdat);abort();

//https://graphics.stanford.edu/~seander/bithacks.html#VariableSignExtend
static inline s64 signex(s64 val, u8 bits){
	const s64 m = 1UL << (bits - 1);
	val = val & ((1UL << bits) - 1);
	return (val ^ m) - m;
}


//Default function that reports a bad instruction
static void func_illegal(u32 opdat){
	fprintf(stderr, "FATAL: Illegal Instruction of %X '%s' full bytes being %X\n", GET_OP(opdat), __PRETTY_FUNCTION__, opdat);
	abort();
}


static void func_tdi(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_twi(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_mulli(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_subfic(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_dozi(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_cmpli(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_cmpi(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_addic(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_addic_dot(u32 opdat){
	TYPE_D
	

	IMP_ME_PLEASE
}

static void func_addi(u32 opdat){
	TYPE_D

	signed_gpr_t register_value = cpu.r[form.RA];
	if(form.RA == 0){ //If the source register is zero, we need to treat this as a value rather than register
		register_value = 0;
	}

	s64 signed_imm = form.SI; //Sign Extend

	cpu.r[form.RT] = register_value + signed_imm;
}

static void func_addis(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_bc_bca_bcl_bcla(u32 opdat){
	TYPE_B

	IMP_ME_PLEASE
}


typedef enum{
	READ,
	WRITE,
	OPEN,
	CLOSE
}SYSCALL_ENUM_t;

#include <unistd.h>
#include <fcntl.h>
static void func_sc(u32 opdat){
	TYPE_SC

	switch(cpu.ru[0]){
		case WRITE:
			cpu.ru[3] = write(cpu.ru[3], &bus.DMA[cpu.ru[4]], cpu.ru[5]);
		break;
		case OPEN:
			cpu.ru[3] = open(&bus.DMA[cpu.ru[3]], cpu.ru[4], cpu.ru[5]);
		break;
		case CLOSE:
			cpu.ru[3] = close(cpu.ru[3]);
		break;

		default:
			fprintf(stderr, "Syscall %u not implemented yet!\n", cpu.ru[0]);
			abort();
		break;	
	}

	IMP_ME_PLEASE
}

static void func_b_ba_bl_bla(u32 opdat){
	TYPE_I

	//TODO: Possibly implement 32bit mode?

	//By default we use absolute addressing
	s64 address_to_jump = form.LI; //Sign extend as well
	if(form.AA == false){
		//Relative addressing
		address_to_jump += cpu.pc;
	}

	if(form.LK == true){
		cpu.lr = cpu.pc + 4;
	}

	cpu.pc = address_to_jump;
}


//########################################################
//					CR_OPS Section
//########################################################
static void func_mcrf(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_bclr(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_crnor(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}


static void func_crandc(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_crxor(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_crnand(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_crand(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_creqv(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_crorc(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_cror(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}

static void func_bcctr(u32 opdat){
	TYPE_XL

	IMP_ME_PLEASE
}





static void func_CR_OP(u32 opdat){
	TYPE_XL

	switch(form.XO){
		case MCRF:
			func_mcrf(opdat);
		break;
		case BCLR: //NOTE: Bclrl is contained in this as well
			func_bclr(opdat);
		break;
		case CRNOR:
			func_crnor(opdat);
		break;
		case CRANDC:
			func_crandc(opdat);
		break;
		case CRXOR:
			func_crxor(opdat);
		break;
		case CRNAND:
			func_crnand(opdat);
		break;
		case CRAND:
			func_crand(opdat);
		break;
		case CREQV:
			func_creqv(opdat);
		break;
		case CRORC:
			func_crorc(opdat);
		break;
		case CROR:
			func_cror(opdat);
		break;
		case BCCTR:
			func_bcctr(opdat);
		break;
		default:
			fprintf(stderr, "FATAL: Bad XO opcode for CR_OP with XO of %X\n", form.XO);
			abort();
		break;
	}
}
//#######################################################
//					CR_OPS END
//########################################################

static void func_rlwimi(u32 opdat){
	TYPE_M

	IMP_ME_PLEASE
}

static void func_rlwinm(u32 opdat){
	TYPE_M
	
	IMP_ME_PLEASE
}

static void func_rlmi(u32 opdat){
	TYPE_M

	IMP_ME_PLEASE
}

static void func_rlwnm(u32 opdat){
	TYPE_M

	IMP_ME_PLEASE
}

static void func_ori(u32 opdat){
	TYPE_D

	u8 RS = form.RT;

	cpu.ru[form.RA] = cpu.ru[RS] | form.UI;
}

static void func_oris(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_xori(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_xoris(u32 opdat){
	TYPE_D
	
	IMP_ME_PLEASE
}

static void func_andi_dot(u32 opdat){
	TYPE_D

	IMP_ME_PLEASE
}

static void func_andis_dot(D_FORM_t opdat){
	TYPE_D

	IMP_ME_PLEASE
}



//###########################################################
//				FX_DWD_ROT Extended Opcodes
//###########################################################

static void func_rldicl(u32 opdat){
	TYPE_MD

	IMP_ME_PLEASE
}

static void func_rldicr(u32 opdat){
	TYPE_MD

	IMP_ME_PLEASE
}

static void func_rldic(u32 opdat){
	TYPE_MD

	IMP_ME_PLEASE
}

static void func_rldimi(u32 opdat){
	TYPE_MD

	IMP_ME_PLEASE
}

static void func_rldcl(u32 opdat){
	TYPE_MD

	IMP_ME_PLEASE
}

static void func_rldcr(u32 opdat){
	TYPE_MD

	IMP_ME_PLEASE
}

static void func_FX_DWD_ROT(u32 opdat){
	TYPE_MD

	//TODO: Measure performance of using a jump table array over a switch jump table
	switch(form.XO){
		case RLDICL:
			func_rldicl(opdat);
		break;
		case RLDICR:
			func_rldicr(opdat);
		break;
		case RLDIC:
			func_rldic(opdat);
		break;
		case RLDIMI:
			func_rldimi(opdat);
		break;
		case RLDCL:
			func_rldcl(opdat);
		break;
		case RLDCR:
			func_rldcr(opdat);
		break;

		default:
			fprintf(stderr, "FATAL: Bad XO opcode for FX_DWD_ROT with XO of %X\n", form.XO);
			abort();
		break;
	}
}

//###########################################################
//			End of FX_DWD_ROT Extended Opcodes
//###########################################################








//###########################################################
//					FX Extended Opcodes
//###########################################################

static void func_cmp(u32 opdat){
	TYPE_X

	IMP_ME_PLEASE
}

static void func_tw(u32 opdat){
	TYPE_X

	IMP_ME_PLEASE
}

static void func_subfc(u32 opdat){
	TYPE_X

	IMP_ME_PLEASE
}

static void func_mulhdu(u32 opdat){
	TYPE_X

	IMP_ME_PLEASE
}

static void func_addc(u32 opdat){
	TYPE_X

	IMP_ME_PLEASE
}

static void func_mulhwu(u32 opdat){
	TYPE_X

	IMP_ME_PLEASE
}

static void func_mfcr(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_ldx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lwzx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_slw(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_cntlzw(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_sld(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_and(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_cmpl(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_subf(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_ldux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lwzux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_cntlzd(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_andc(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_td(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_mulhd(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_mulhw(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lbzx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_neg(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lbzux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_nor(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_subfe(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_adde(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_mtcrf(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stdx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stwx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stdux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stwux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_subfze(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_addze(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stbx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_subfme(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_mulld(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_addme(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_mullw(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stbux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_add(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lhzx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_eqv(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lhzux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_xor(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_mfspr(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lwax(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lhax(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lwaux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lhaux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_sthx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_orc(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_sradi(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_sthux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_or(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_divdu(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_divwu(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_mtspr(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_nand(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_divd(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_divw(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_mcrxr(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lswx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lwbrx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lfsx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_srw(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_srd(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lfsux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lswi(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lfdx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lfdux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stswx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stwbrx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stfsx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stfsux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stswi(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stfdx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stfdux(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_lhbrx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_sraw(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_srad(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_srawi(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_sthbrx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_extsh(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_extsb(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_stfiwx(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}

static void func_extsw(u32 opdat){
	TYPE_X
	
	IMP_ME_PLEASE
}






static void func_FX(u32 opdat){
	TYPE_X

	switch(form.XO){
		case CMP:
			func_cmp(opdat);
		break;
		case TW:
			func_tw(opdat);
		break;
		case SUBFC:
			func_subfc(opdat);
		break;
		case MULHDU:
			func_mulhdu(opdat);
		break;
		case ADDC:
			func_addc(opdat);
		break;
		case MULHWU:
			func_mulhwu(opdat);
		break;
		case MFCR:
			func_mfcr(opdat);
		break;
		case LDX:
			func_ldx(opdat);
		break;
		case LWZX:
			func_lwzx(opdat);
		break;
		case SLW:
			func_slw(opdat);
		break;
		case CNTLZW:
			func_cntlzw(opdat);
		break;
		case SLD:
			func_sld(opdat);
		break;
		case AND:
			func_and(opdat);
		break;
		case CMPL:
			func_cmpl(opdat);
		break;
		case SUBF:
			func_subf(opdat);
		break;
		case LDUX:
			func_ldux(opdat);
		break;
		case LWZUX:
			func_lwzux(opdat);
		break;
		case CNTLZD:
			func_cntlzd(opdat);
		break;
		case ANDC:
			func_andc(opdat);
		break;
		case TD:
			func_td(opdat);
		break;
		case MULHD:
			func_mulhd(opdat);
		break;
		case MULHW:
			func_mulhw(opdat);
		break;
		case LBZX:
			func_lbzx(opdat);
		break;
		case NEG:
			func_neg(opdat);
		break;
		case LBZUX:
			func_lbzux(opdat);
		break;
		case NOR:
			func_nor(opdat);
		break;
		case SUBFE:
			func_subfe(opdat);
		break;
		case ADDE:
			func_adde(opdat);
		break;
		case MTCRF:
			func_mtcrf(opdat);
		break;
		case STDX:
			func_stdx(opdat);
		break;
		case STWX:
			func_stwx(opdat);
		break;
		case STDUX:
			func_stdux(opdat);
		break;
		case STWUX:
			func_stwux(opdat);
		break;
		case SUBFZE:
			func_subfze(opdat);
		break;
		case ADDZE:
			func_addze(opdat);
		break;
		case STBX:
			func_stbx(opdat);
		break;
		case SUBFME:
			func_subfme(opdat);
		break;
		case MULLD:
			func_mulld(opdat);
		break;
		case ADDME:
			func_addme(opdat);
		break;
		case MULLW:
			func_mullw(opdat);
		break;
		case STBUX:
			func_stbux(opdat);
		break;
		case ADD:
			func_add(opdat);
		break;
		case LHZX:
			func_lhzx(opdat);
		break;
		case EQV:
			func_eqv(opdat);
		break;
		case LHZUX:
			func_lhzux(opdat);
		break;
		case XOR:
			func_xor(opdat);
		break;
		case MFSPR:
			func_mfspr(opdat);
		break;
		case LWAX:
			func_lwax(opdat);
		break;
		case LHAX:
			func_lhax(opdat);
		break;
		case LWAUX:
			func_lwaux(opdat);
		break;
		case LHAUX:
			func_lhaux(opdat);
		break;
		case STHX:
			func_sthx(opdat);
		break;
		case ORC:
			func_orc(opdat);
		break;
		case SRADI:
			func_sradi(opdat);
		break;
		case STHUX:
			func_sthux(opdat);
		break;
		case OR:
			func_or(opdat);
		break;
		case DIVDU:
			func_divdu(opdat);
		break;
		case DIVWU:
			func_divwu(opdat);
		break;
		case MTSPR:
			func_mtspr(opdat);
		break;
		case NAND:
			func_nand(opdat);
		break;
		case DIVD:
			func_divd(opdat);
		break;
		case DIVW:
			func_divw(opdat);
		break;
		case MCRXR:
			func_mcrxr(opdat);
		break;
		case LSWX:
			func_lswx(opdat);
		break;
		case LWBRX:
			func_lwbrx(opdat);
		break;
		case LFSX:
			func_lfsx(opdat);
		break;
		case SRW:
			func_srw(opdat);
		break;
		case SRD:
			func_srd(opdat);
		break;
		case LFSUX:
			func_lfsux(opdat);
		break;
		case LSWI:
			func_lswi(opdat);
		break;
		case LFDX:
			func_lfdx(opdat);
		break;
		case LFDUX:
			func_lfdux(opdat);
		break;
		case STSWX:
			func_stswx(opdat);
		break;
		case STWBRX:
			func_stwbrx(opdat);
		break;
		case STFSX:
			func_stfsx(opdat);
		break;
		case STFSUX:
			func_stfsux(opdat);
		break;
		case STSWI:
			func_stswi(opdat);
		break;
		case STFDX:
			func_stfdx(opdat);
		break;
		case STFDUX:
			func_stfdux(opdat);
		break;
		case LHBRX:
			func_lhbrx(opdat);
		break;
		case SRAW:
			func_sraw(opdat);
		break;
		case SRAD:
			func_srad(opdat);
		break;
		case SRAWI:
			func_srawi(opdat);
		break;
		case STHBRX:
			func_sthbrx(opdat);
		break;
		case EXTSH:
			func_extsh(opdat);
		break;
		case EXTSB:
			func_extsb(opdat);
		break;
		case STFIWX:
			func_stfiwx(opdat);
		break;
		case EXTSW:
			func_extsw(opdat);
		break;

		default:
			fprintf(stderr, "ERR: FX Invalid Extended Opcode %X with full bytes of %X\n", GET_OP(opdat), opdat);
			abort();
		break;
	}
}


//###########################################################
//				END of FX Extended Opcodes
//###########################################################


static void func_lwz(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lwzu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lbz(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lbzu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stw(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stwu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stb(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stbu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lhz(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lhzu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lha(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lhau(u32 opdat){
	IMP_ME_PLEASE
}

static void func_sth(u32 opdat){
	IMP_ME_PLEASE
}

static void func_sthu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lmw(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stmw(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lfs(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lfsu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lfd(u32 opdat){
	IMP_ME_PLEASE
}

static void func_lfdu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stfs(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stfsu(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stfd(u32 opdat){
	IMP_ME_PLEASE
}

static void func_stfdu(u32 opdat){
	IMP_ME_PLEASE
}


//#########################################################################
//			FX_DS_FORM_LOADS Extended Opcodes
//#########################################################################

static void func_ld(u32 opdat){
	TYPE_DS

	IMP_ME_PLEASE
}

static void func_ldu(u32 opdat){
	TYPE_DS

	IMP_ME_PLEASE	
}

static void func_lwa(u32 opdat){
	TYPE_DS

	IMP_ME_PLEASE
}


static void func_FX_DS_FORM_LOADS(u32 opdat){
	TYPE_DS

	switch(form.XO){
		case LD:
			func_ld(opdat);
		break;
		case LDU:
			func_ldu(opdat);
		break;
		case LWA:
			func_lwa(opdat);
		break;

		default:
			fprintf(stderr, "ERROR: Invalid XO for FX_DS_FORM_LOADS of %X with full bytes of %X\n", form.XO, opdat);
			abort();		
		break;
	}
}

//#########################################################################
//			End of FX_DS_FORM_LOADS Extended Opcodes
//#########################################################################








//#########################################################################
//			FP_SINGLE Extended Opcodes
//#########################################################################



static void func_FP_SINGLE(u32 opdat){
	IMP_ME_PLEASE
}

//#########################################################################
//			End of FP_SINGLE Extended Opcodes
//#########################################################################










//#########################################################################
//			FX_DS_FORM_STORES Extended Opcodes
//#########################################################################



static void func_FX_DS_FORM_STORES(u32 opdat){
	IMP_ME_PLEASE
}


//#########################################################################
//			End of FX_DS_FORM_STORES Extended Opcodes
//#########################################################################












//#########################################################################
//			FP_DOUBLE Extended Opcodes
//#########################################################################


static void func_FP_DOUBLE(u32 opdat){
	IMP_ME_PLEASE
}


//#########################################################################
//			End of FP_DOUBLE Extended Opcodes
//#########################################################################




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
	TYPE_SC
	
	int err = 0;

	//TODO: redo this
	abort();
	switch(1){
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



//A list of function pointers to the corresponding opcode id
static const microcode_t OPCODE_MICRCODE[OPCODE_LEN] = {
	func_illegal,
	func_illegal,
	func_tdi,
	func_twi,
	func_illegal,
	func_illegal,
	func_illegal,
	func_mulli,
	func_subfic,
	func_dozi,
	func_cmpli,
	func_cmpi,
	func_addic,
	func_addic_dot,
	func_addi,
	func_addis,
	func_bc_bca_bcl_bcla,
	func_sc,
	func_b_ba_bl_bla,
	func_CR_OP,
	func_rlwimi,
	func_rlwinm,
	func_rlmi,
	func_rlwnm,
	func_ori,
	func_oris,
	func_xori,
	func_xoris,
	func_andi_dot,
	func_andis_dot,
	func_FX_DWD_ROT,
	func_FX,
	func_lwz,
	func_lwzu,
	func_lbz,
	func_lbzu,
	func_stw,
	func_stwu,
	func_stb,
	func_stbu,
	func_lhz,
	func_lhzu,
	func_lha,
	func_lhau,
	func_sth,
	func_sthu,
	func_lmw,
	func_stmw,
	func_lfs,
	func_lfsu,
	func_lfd,
	func_lfdu,
	func_stfs,
	func_stfsu,
	func_stfd,
	func_stfdu,
	func_illegal,
	func_illegal,
	func_FX_DS_FORM_LOADS,
	func_FP_SINGLE,
	func_illegal,
	func_illegal,
	func_FX_DS_FORM_STORES,
	func_FP_DOUBLE
};

const char* OPCODE_NAMES[OPCODE_LEN] = {
	"ILLEGAL",
	"ILLEGAL_01",
	"TDI",
	"TWI",
	"ILLEGAL_04",
	"ILLEGAL_05",
	"ILLEGAL_06",
	"MULLI",
	"SUBFIC",
	"DOZI",
	"CMPLI",
	"CMPI",
	"ADDIC",
	"ADDIC.",
	"ADDI",
	"ADDIS",
	"BC",
	"SC",
	"B",
	"CR_OP",
	"RLWIMI",
	"RLWINM",
	"RLMI",
	"RLWNM",
	"ORI",
	"ORIS",
	"XORI",
	"XORIS",
	"ANDI.",
	"ANDIS.",
	"FX_DWD_ROT",
	"FX",
	"LWZ",
	"LWZU",
	"LBZ",
	"LBZU",
	"STW",
	"STWU",
	"STB",
	"STBU",
	"LHZ",
	"LHZU",
	"LHA",
	"LHAU",
	"STH",
	"STHU",
	"LMW",
	"STMW",
	"LFS",
	"LFSU",
	"LFD",
	"LFDU",
	"STFS",
	"STFSU",
	"STFD",
	"STFDU",
	"ILLEGAL_56",
	"ILLEGAL_57",
	"FX_DS_FORM_LOADS",
	"FP_SINGLE",
	"ILLEGAL_60",
	"ILLEGAL_61",
	"FX_DS_FORM_STORES",
	"FP_DOUBLE",	
};






#include <string.h>
microcode_t decode_opcode_name(const char* restrict name){
	for(unsigned char i = 0; i < OPCODE_LEN; i++){
		if(strcmp(name, OPCODE_NAMES[i]) == 0){
			return OPCODE_MICRCODE[i];
		}
	}

	//If we cant find the instruction then return an illegal instruction
	return func_illegal;
}

//Based off of binary return the struct for the function pointer to run
microcode_t decode_opcode_bytes(u32 bytes){
	OPCODE_BASE_t base_opcode = GET_OP(bytes);
	if(base_opcode >= OPCODE_BASE_T__SIZE){
		fprintf(stderr, "ERR: Invalid base opcode of %u\n", base_opcode);
		exit(EXIT_FAILURE);	
	}

	return OPCODE_MICRCODE[base_opcode];
}



INLINE char* opcode_to_name(unsigned char opcode){
	return strdup(OPCODE_NAMES[opcode]);
}


//This is disabled until I can re-write this
#if 0

//Encode a single byte of data in the dump
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

//Check if there is a duplicate entry in the vector 'v'
void simplify_memory_vector(vector* v){
	for(fu_index i = 0; i < vector_size(v); i++){
		fu_BinFile* region = (fu_BinFile*)vector_index(v, i);
		for(fu_index j = 0; j < vector_size(v); j++){
			if(i == j) continue; //The same entry will always 
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
				i--;
				break; //Exit the 'j' for loop and return to the 'i' for loop
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
		//TODO: What the heck is this for
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
#endif



void init_opcodes(){
	//#ifdef LOG
	execution_log.size = 32;
	execution_log.text = (char**)malloc(sizeof(char*) * execution_log.size);
	//#endif

	for(u8 i = 0; i < execution_log.size; i++){
		//#ifdef LOG
		execution_log.text[i] = NULL;
		//#endif
	}
}
