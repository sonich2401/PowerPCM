#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cstr.h"
#include "vector.h"
#include "types.h"
#include "../opcode.h"
#include "../bus.h"

#include "linker.h"
#include "asm.h"



static opcode_t func_I(u32 op, u32 li, u32 aa, u32 lk){
	I_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

static opcode_t func_B(u32 op, u32 bo, u32 bi, u32 bd, u32 aa, u32 lk){
	B_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero	

	return *((opcode_t*)&form);
}

static opcode_t func_SC(u32 op){
	SC_FORM_t form;
	*((u32*)&form) = 0; //Set all bytes to zero
	form.opcode = op;
	form.always_one = 1;

	return *((opcode_t*)&form);
}


//####################################################################################
//					D FORM
//####################################################################################


static opcode_t func_D1(u32 op, u32 rt, u32 ra, u32 d){
	D_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}


static opcode_t func_D2(u32 op, u32 bf, u32 l, u32 ra, u32 d){
	D_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

//####################################################################################
//					D FORM
//####################################################################################


static opcode_t func_DS(u32 op, u32 rt, u32 ra, u32 ds, u32 xo){
	DS_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}



//####################################################################################
//					X FORM
//####################################################################################



static opcode_t func_X1(u32 op, u32 rt, u32 ra, u32 rb, u32 xo, u32 rc){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

static opcode_t func_X2(u32 op, u32 rt, u32 sr, u32 xo){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

static opcode_t func_X3(u32 op, u32 rs, u32 l, u32 xo){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero
	
	return *((opcode_t*)&form);
}

static opcode_t func_X4(u32 op, u32 bf, u32 l, u32 ra, u32 rb, u32 xo){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

static opcode_t func_X5(u32 op, u32 bf, u32 fra, u32 frb, u32 xo){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

static opcode_t func_X6(u32 op, u32 bf, u32 u, u32 xo, u32 rc){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

static opcode_t func_X7(u32 op, u32 th, u32 ra, u32 rb, u32 xo){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

static opcode_t func_X8(u32 op, u32 l, u32 ra, u32 rb, u32 xo){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero
	
	return *((opcode_t*)&form);
}

static opcode_t func_X9(u32 op, u32 big_l, u32 xo){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

static opcode_t func_X10(u32 op, u32 to, u32 ra, u32 rb, u32 xo){
	X_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	return *((opcode_t*)&form);
}

//####################################################################################
//					XL FORM
//####################################################################################


static opcode_t func_XL1(u32 op, u32 bt, u32 ba, u32 bb, u32 xo){
	XL_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}

static opcode_t func_XL2(u32 op, u32 bt, u32 ba, u32 bh, u32 xo, u32 lk){
	XL_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}

static opcode_t func_XL3(u32 op, u32 bf, u32 bfa, u32 xo){
	XL_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}

static opcode_t func_XL4(u32 op, u32 xo){
	XL_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}

//####################################################################################
//					XFX FORM
//####################################################################################


static opcode_t func_XFX1(u32 op, u32 rt, u32 spr, u32 xo){
	XFX_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}

static opcode_t func_XFX2(u32 op, u32 rt, u32 xo){
	XFX_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}

static opcode_t func_XFX3(u32 op, u32 rt, u32 fxm, u32 xo){
	XFX_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}

static opcode_t func_XFX4(u32 op, u32 rt, u32 fxm, u32 xo){
	XFX_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}

static opcode_t func_XFX5(u32 op, u32 rt, u32 fxm, u32 xo){
	XFX_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}


//####################################################################################
//					XFL FORM
//####################################################################################

static opcode_t func_XFL(u32 op, u32 flm, u32 frb, u32 xo, u32 rc){
	XFL_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}


//####################################################################################
//					XS FORM
//####################################################################################

static opcode_t func_XS(u32 op, u32 rs, u32 ra, u32 sh, u32 xo, u32 sh1, u32 rc){
	XS_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}



//####################################################################################
//					XO FORM
//####################################################################################

static opcode_t func_XO(u32 op, u32 rt, u32 ra, u32 rb, u32 oe, u32 xo, u32 rc){
	XO_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}


//####################################################################################
//					A FORM
//####################################################################################

static opcode_t func_A(u32 op, u32 frt, u32 fra, u32 frc, u32 xo, u32 rc){
	A_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}




//####################################################################################
//					M FORM
//####################################################################################

static opcode_t func_M(u32 op, u32 rs, u32 ra, u32 rb, u32 mb, u32 me, u32 rc){
	M_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}


//####################################################################################
//					MD FORM
//####################################################################################

static opcode_t func_MD(u32 op, u32 rs, u32 ra, u32 sh, u32 mb, u32 xo, u32 sh1, u32 rc){
	M_FORM_t form;
	*((opcode_t*)&form) = 0; //Set all bytes to zero

	form.opcode = op;

	return *((opcode_t*)&form);
}





//####################################################################################
//					FP_DOUBLE FORM
//####################################################################################


//There is a mix of "X" and "A" forms so we need to figure it out
static opcode_t func_FP_DOUBLE(u32 op, u32 a, u32 b, u32 c, u32 d, u32 e){
	//Mode "A"
	if(op >= 18 || op <= 31){
		return func_A(op, a, b, c, d, e);
	}

	//Mode "X"
	return func_X1(op, a, b, c, d, e);
}



//TODO: Delete me
#define func_D func_D1


#define func_illegal NULL
#define func_tdi func_D
#define func_twi func_D
#define func_mulli func_D
#define func_subfic func_D
#define func_dozi func_D
#define func_cmpli func_D
#define func_cmpi func_D
#define func_addic func_D
#define func_addic_dot func_D
#define func_addi func_D
#define func_addis func_D
#define func_bc_bca_bcl_bcla func_B
#define func_sc func_SC
#define func_b_ba_bl_bla func_I
#define func_CR_OP func_XL1
#define func_rlwimi func_M
#define func_rlwinm func_M
#define func_rlmi func_M
#define func_rlwnm func_M
#define func_ori func_D
#define func_oris func_D
#define func_xori func_D
#define func_xoris func_D
#define func_andi_dot func_D
#define func_andis_dot func_D
#define func_FX_DWD_ROT func_MD
#define func_FX func_X1
#define func_lwz func_D
#define func_lwzu func_D
#define func_lbz func_D
#define func_lbzu func_D
#define func_stw func_D
#define func_stwu func_D
#define func_stb func_D
#define func_stbu func_D
#define func_lhz func_D
#define func_lhzu func_D
#define func_lha func_D
#define func_lhau func_D
#define func_sth func_D
#define func_sthu func_D
#define func_lmw func_D
#define func_stmw func_D
#define func_lfs func_D
#define func_lfsu func_D
#define func_lfd func_D
#define func_lfdu func_D
#define func_stfs func_D
#define func_stfsu func_D
#define func_stfd func_D
#define func_stfdu func_D
#define func_FX_DS_FORM_LOADS func_DS
#define func_FP_SINGLE func_A
#define func_FX_DS_FORM_STORES func_DS


//Extended ops need a bit of help to encode them.






typedef u32 (*asm_microcode_t)(u32, u32, u32, u32, u32, u32, u32, u32);

const asm_microcode_t ASM_OPCODE_PTR[] = {
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


const char* ASM_OPCODE_NAMES[] = {
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

	//CR_OPS
	"MCRF",
	"BCLR",
	"CRNOR",
	"CRANDC",
	"CRXOR",
	"CRNAND",
	"CRAND",
	"CREQV",
	"CRORC",
	"CROR",
	"BCCTR",

	//FX_DWD_ROT_EXT
	"RLDICL",
	"RLDICR",
	"RLDIC",
	"RLDIMI",
	"RLDCL",
	"RLDCR",

	//FX_EXT
	"CMP",
	"TW",
	"SUBFC",
	"MULHDU",
	"ADDC",
	"MULHWU",
	"MFCR",
	"LDX",
	"LWZX",
	"SLW",
	"CNTLZW",
	"SLD",
	"AND",
	"CMPL",
	"SUBF",
	"LDUX",
	"LWZUX",
	"CNTLZD",
	"ANDC",
	"TD",
	"MULHD",
	"MULHW",
	"LBZX",
	"NEG",
	"LBZUX",
	"NOR",
	"SUBFE",
	"ADDE",
	"MTCRF",
	"STDX",
	"STWX",
	"STDUX",
	"STWUX",
	"SUBFZE",
	"ADDZE",
	"STBX",
	"SUBFME",
	"MULLD",
	"ADDME",
	"MULLW",
	"STBUX",
	"ADD",
	"LHZX",
	"EQV",
	"LHZUX",
	"XOR",
	"MFSPR",
	"LWAX",
	"LHAX",
	"LWAUX",
	"LHAUX",
	"STHX",
	"ORC",
	"SRADI",
	"STHUX",
	"OR",
	"DIVDU",
	"DIVWU",
	"MTSPR",
	"NAND",
	"DIVD",
	"DIVW",
	"MCRXR",
	"LSWX",
	"LWBRX",
	"LFSX",
	"SRW",
	"SRD",
	"LFSUX",
	"LSWI",
	"LFDX",
	"LFDUX",
	"STSWX",
	"STWBRX",
	"STFSX",
	"STFSUX",
	"STSWI",
	"STFDX",
	"STFDUX",
	"LHBRX",
	"SRAW",
	"SRAD",
	"SRAWI",
	"STHBRX",
	"EXTSH",
	"EXTSB",
	"STFIWX",
	"EXTSW",
};



const size_t ASM_OPCODE_LEN = sizeof(ASM_OPCODE_PTR)/sizeof(asm_microcode_t);


u32 name_to_instr(const char* __restrict__ memonic){
  char* name = cstrdup_stack(memonic);
  char* ogptr = name;
  //Normalize name
  while(*name != 0){
    if(*name == ' ') {
      *name = 0; 
      break;
    }
    if(*name > 0x5F){
      *name -= ('a' - 'A');
    }

    name++;
  }
  name = ogptr;
  
  //Search for opcode
  for(unsigned int i = 0; i < OPCODE_LEN; i++){
    if(strcmp(name, OPCODE_NAMES[i]) == 0){
      return i;
    }
  }

  fprintf(stderr, "ERR: Unknown memonic of \"%s\"\n", memonic);
  abort();
  return 0;
}




//Add opcode to bin and then move the cursor to the next location
static INLINE char* push32(char* __restrict__ buff, u32 opdat){
  *((u32*)buff) = opdat;
  buff += sizeof(u32);
  return buff;
}

#define PUSH32(val) tmp_bin = push32(tmp_bin, val);


//finds '\' 'n' and converts it to the single character of '\n'
static void decode_backslashes(cstr str){
  cstr og_string = str;
  while((str = strchr(str, '\\')) != NULL){
    cstrshl(str, 1); //Remove \ character
    
    switch(*str){
      case 'n':
        *str = '\n';
      break;
      case 'b':
        *str = '\b';
      break;
      case 't':
        *str = '\t';
      break;
      case 'r':
        *str = '\r';
      break;
      case '0':
        cstrshl(str, 1); //Count as a spacebar
      break;
      
      default:
        fprintf(stderr, "ERR ASM: Current string contains a special character that I do not know how to interperut! str = \"%s\"\n", og_string);
        abort();
      break;
    }
  }
}



//Possible future use for a proper linker?
typedef struct{
  vector labels;
  Asm_file_t asm_global;
}Intermediate_Asm_File_t;

static u64 pc; //Used to find where to place the data
extern char logging;

static Intermediate_Asm_File_t get_labels(fu_TextFile* txt, vector* __restrict__ predefined){
  pc = 4; //For some reason it needs to be 8 instead of 4?
  u64 start_pc = 0;//Set the rom start address to 0. If this stays zero after all labels are decoded then 'start' was never defined
  vector labels;
  vector globals = vector_create(sizeof(Label), free_label);
  
  //Add the predefined labels
  if(predefined){
    labels = *predefined;
  }else{
    labels = vector_create(sizeof(Label), free_label);
  }
  
  Intermediate_Asm_File_t ret;
  ret.asm_global.bin = fu_alloc_bin_file(sizeof(u32));
  
  for(u64 i = 0; i < txt->size; i++){
    char* cursor = strchr(txt->text[i], ':');
    if(!cursor) continue;
    if(cstrcnt(txt->text[i], "\"") == 2 && strstr(txt->text[i], ".file ") != txt->text[i]){
      //Remove quotes and copy binary data
      fu_BinFile dat_seg;
      dat_seg.bin = strdup(strchr(txt->text[i], '"') + 1);
      *(strrchr(dat_seg.bin, '"')) = 0;
      
      //Decode backslash characters
      decode_backslashes(dat_seg.bin);
      //Copy into executable binary
      dat_seg.size = strlen(dat_seg.bin) + 1;
      fu_join_bin(&ret.asm_global.bin, dat_seg);

      
      //Add label
      Label tmp = Label_construct(strdup(txt->text[i]), pc + ROM_START); //Add +ROM_START because DMA is not relitive to cpu.pc
      *(strchr(tmp.str, ' ') - 1) = 0; //remove ':' character
      labels.push_back(&labels, &tmp);
      pc += dat_seg.size;
      fu_delete_text(txt, i); //Remove line that contained the label
      i--;
      
      fu_free_bin_file(dat_seg);
      continue;
    }
  
    //.file testFile: "/home/rendev/Desktop/myFile.s"
    if(strlen(cursor) > 2 && strstr(txt->text[i], ".file ") == txt->text[i]){  //Number is after the label
      if(cstrcnt(txt->text[i], "\"") != 2){
        fprintf(stderr, "ERR: Line #%llu: \"%s\" does not have double quotes for the file name!\n", i, txt->text[i]);
        exit(EXIT_FAILURE);
      }

      char* fp = cstrdup_stack(strchr(txt->text[i], '"') + 1);
      *(strrchr(fp, '"')) = 0;
      char* tmp_cursor = strstr(fp, ".png");
      bool isSprite = false;
      if(tmp_cursor != NULL){
        isSprite = strlen(tmp_cursor) == strlen(".png"); //Does the file have a .png exension
      }

      fu_BinFile bin = fu_load_bin_file(fp);

      //add the file byte count to the constant data
      fu_BinFile dat_seg = fu_alloc_bin_file(sizeof(fu_index));
      *(fu_index*)dat_seg.bin = bin.size;

      fu_join_bin(&ret.asm_global.bin, dat_seg);
      fu_join_bin(&ret.asm_global.bin, bin);


      
      //Add label
      Label tmp = Label_construct(strdup(strchr(txt->text[i], ' ') + 1), pc + ROM_START); //Add +ROM_START because DMA is not relitive to cpu.pc
      *(strchr(tmp.str, ' ') - 1) = 0; //remove ':' character
      labels.push_back(&labels, &tmp);
      pc += dat_seg.size;
      pc += bin.size;
      fu_delete_text(txt, i); //Remove line that contained the label
      i--;

      fu_free_bin_file(dat_seg);
      fu_free_bin_file(bin);
      continue;
    }

    //.macro testMacro: 20
    if(strlen(cursor) > 2 && strstr(txt->text[i], ".macro ") == txt->text[i]){  //Number is after the label

      unsigned int tmpint = 0;
      sscanf(cursor + 1, "%u", &tmpint);
      
      //Add label
      Label tmp = Label_construct(strdup(strchr(txt->text[i], ' ') + 1), tmpint);
      *(strchr(tmp.str, ' ') - 1) = 0; //remove ':' character
      labels.push_back(&labels, &tmp);
      fu_delete_text(txt, i); //Remove line that contained the label
      i--;
      
      continue;
    }
    
    //testInt: 210
    if(strlen(cursor) > 2){  //Number is after the label
      
      fu_BinFile dat_seg;
      dat_seg.bin = (char*)malloc(sizeof(unsigned int));
      sscanf(cursor + 1, "%u", (unsigned int*)dat_seg.bin);
      
      //Copy into executable binary
      dat_seg.size = sizeof(unsigned int);
      fu_join_bin(&ret.asm_global.bin, dat_seg);

      
      //Add label
      Label tmp = Label_construct(strdup(txt->text[i]), pc + ROM_START); //Add +ROM_START because DMA is not relitive to cpu.pc
      *(strchr(tmp.str, ' ') - 1) = 0; //remove ':' character
      labels.push_back(&labels, &tmp);
      pc += dat_seg.size;
      fu_delete_text(txt, i); //Remove line that contained the label
      i--;
      
      fu_free_bin_file(dat_seg);
      continue;
    }
  }
  
  fu_index data_size = pc;

  
  for(u64 i = 0; i < txt->size; i++){
    pc = i * 4 + data_size;
    if(strstr(txt->text[i], ":") != NULL){
      Label tmp = Label_construct(strdup(txt->text[i]), pc);
      tmp.str[strlen(tmp.str) - 1] =0;
      if(logging){
        printf("LABEL: \"%s\": %llX\n", tmp.str, tmp.ptr);
      }
      labels.push_back(&labels, &tmp);
      //printf("LBL \"%s\" at %llu\n", tmp.str, pc);
      if(strcmp(tmp.str, "start") == 0){
        start_pc = pc;
      }
      fu_delete_text(txt, i);
      i--;
      continue;
    }
    #if 0
    if(strstr(txt->text[i], ".global") != NULL){
      Label tmp = Label_construct(strdup(txt->text[i]), pc);
      tmp.str[strlen(tmp.str) - 1] = 0;
      globals.push_back(&globals, &tmp);
      fu_delete_text(txt, i);
      i--;
      continue;
    }
    #endif
    
    //This line of the file was assembly code
    
  }
  
  if(start_pc == 0){
    fprintf(stderr, "ASM ERR: 'start' label was not found!\n");
    abort();
  }else{
    *((u32*)ret.asm_global.bin.bin)  = ((u32)(start_pc)); //Set start rom address
  }

  ret.labels = labels;
  ret.asm_global.globals = globals;
  return ret;
}

static void remove_register_indicators(cstr str){
  if(!str) return;
  cstr tmp = str;
   
  while((tmp = strstr(str, "sp"))){
    //Convert sp to r0
    tmp[0] = 'r';
    tmp[1] = '0';
  }

  while((tmp = strchr(str, 'r')) != NULL){
    if(!(tmp[1] >= '0' && tmp[1] <= '9')) break; //Invalid characters found! isBadString will resolve this later
    cstrshl(tmp, 1);  //Remove the 'r'
  }

  while((tmp = strchr(str, 'f')) != NULL){
    cstrshl(tmp, 1);  //Remove the 'f'
  }
  
  while((tmp = strchr(str, '$')) != NULL){
    cstrshl(tmp, 1);  //Remove the '$'
  }
  
  while((tmp = strchr(str, '%')) != NULL){
    cstrshl(tmp, 1);  //Remove the '%'
  }
}


//Checks the string for any non number characters
static bool isBadString(cstr str){
	if(!(str = strchr(str, ' ')) ) return false; //Checks for the first spacebar. If there is no spacebar it returns false
	while(*str != 0){
		if(*str >= 'a' && *str <= 'z') return true;
		if(*str >= 'a' && *str <= 'z') return true;
		if(*str == '-') return true; //Disable negitive numbers for now
		if(!(*str >= '0' && *str <= '9') && (*str != ' ')) return true;
		str++;
	}
	return false;
}


static void clean_text(fu_TextFile* t){
  for(fu_index i = 0; i < t->size; i++){
    //Remove unwanted text
    char* tmp = strrchr(t->text[i], '(');
    if(tmp) *tmp = ' ';
    cstrrem(t->text[i], ',');
    cstrrem(t->text[i], ')');
    char* new_str = cstrrep(t->text[i], "(", " ");
    free(t->text[i]);
    t->text[i] = new_str;
    tmp = t->text[i];
    while((tmp = strstr(t->text[i], "  "))){ //remove all double spaces
      char* cursor = tmp;
      unsigned long char_count = 0;
      while(*cursor == ' ') {char_count++; cursor++;}
      cstrshl(tmp, char_count - 1); //remove all but 1 space
    }
    while(t->text[i][0] == ' '){ //Remove spaces from begining
      cstrshl(t->text[i], 1);
    }
    while(strrchr(t->text[i], '\t')){ //remove all tabs
      cstrrem(t->text[i], '\t');
    }
    while(strrchr(t->text[i], '\n')){ //remove all newlines (should never happen)
      cstrrem(t->text[i], '\n');
    }
    while(strrchr(t->text[i], '\r')){ //remove all return carrages (windows compatibility)
      cstrrem(t->text[i], '\r');
    }
    tmp = strchr(t->text[i], ';'); //Remove comments
    if(tmp != NULL){
      *tmp = 0;
    }

    //remove extra spaces at the end of text
    unsigned long text_len = strlen(t->text[i]);
    if(text_len > 0){ 
      while(t->text[i][text_len - 1] == ' '){
        t->text[i][text_len - 1] = 0;
        text_len--;
        if(text_len == 0) break;
      }
    }

    if(strlen(t->text[i]) == 0){ //Remove blank lines
      fu_delete_text(t, i);
      i--; // i - 1 + 1 = i
      continue;
    }
    //puts(t->text[i]);
  }
}

char* check_and_replace_label(cstr ele, Intermediate_Asm_File_t asm_f) {
  #define labels asm_f.labels
  //replace label with memory address
  for (u64 i1 = 0; i1 < labels.size(&labels); i1++) {
    Label* ele1 = (Label*)labels.index(&labels, i1);
    char* cache = strstr(ele, ele1->str);
    if (cache != NULL && *(cache - 1) == ' ') {
      char* tmp_string = cstrdup_stack(cache);
      
      char* helper;
      if ((helper = strchr(tmp_string, ' '))) {
        *helper = 0;
      }

      if (strcmp(tmp_string, ele1->str) != 0) {
        continue;
      }
      
      char num_buffer[50];
      sprintf(num_buffer, "%llu", ele1->ptr);
      ele = cstrrep(ele, ele1->str, num_buffer);
      break;
    }
  }
  #undef labels
  return ele;
}


#define PUSH_LABEL(key, val) tmp_label = Label_construct(strdup(key), val); ret.push_back(&ret, &tmp_label)

static vector get_predefined_labels(){
  vector ret;
  vector_init(&ret, sizeof(Label), free_label);
  Label tmp_label;
  
  PUSH_LABEL("__STACK_TOP__", STACK_SIZE);
  PUSH_LABEL("___MALLOC__BLOCK__", STACK_SIZE);
  PUSH_LABEL("__FRAMEBUFFER_START__", FRAMEBUFFER_START);
  PUSH_LABEL("__FRAMEBUFFER_END__", FRAMEBUFFER_END);
  PUSH_LABEL("__FRAMEBUFFER_WIDTH__", SCREEN_W);
  PUSH_LABEL("__FRAMEBUFFER_HEIGHT__", SCREEN_H);
  
  PUSH_LABEL("true", 1);
  PUSH_LABEL("TRUE", 1);
  PUSH_LABEL("True", 1);
  
  PUSH_LABEL("false", 0);
  PUSH_LABEL("FALSE", 0);
  PUSH_LABEL("False", 0);
  
  PUSH_LABEL("NULL", 0);
  PUSH_LABEL("null", 0);
  PUSH_LABEL("nullptr", 0);
  
  //ret.push_back(&ret, &tmp_label);
  return ret;
}

#undef PUSH_LABEL





//MAIN ASSEMBLING CODE


fu_BinFile assemble_s(fu_TextFile assembly){
  if(logging){
    puts("ASSEMBLING ...");
  }
  
  fu_TextFile t = fu_create_text_copy(assembly);
  clean_text(&t); //Remove all junk characters
  
  vector predefined_labels = get_predefined_labels();
  Intermediate_Asm_File_t tmp_asm = get_labels(&t, &predefined_labels); //resolve all labels and macros
  
  //Alloc only nessisary size  
  fu_BinFile ret = fu_alloc_bin_file((tmp_asm.asm_global.bin.size + ((t.size + 2) * 4)) % ROM_SIZE);  
  char* tmp_bin = ret.bin;
  
  //Copy data segments into rom including start addr
  memcpy(ret.bin, tmp_asm.asm_global.bin.bin, tmp_asm.asm_global.bin.size);
  tmp_bin += tmp_asm.asm_global.bin.size;
  u64 start_addr = tmp_asm.asm_global.bin.size;
  
  
  for(fu_index i = 0; i < t.size; i++){
    //Replace label if it exists
    t.text[i] = check_and_replace_label(t.text[i], tmp_asm);
    
    //Remove register indicators (aka. r13 or $r32, or %r2)
    remove_register_indicators(strchr(t.text[i], ' '));

    #ifndef DISABLE_ERR_CHECKING
    if(isBadString(t.text[i])){
      fprintf(stderr, "ERR: Assembler: Line %llu of the file contains invalid characters!\nAssembly dump is as follows which shows the offending code ...\n", i);
      while((i   - 2) >= t.size){i++;}
      for(fu_index i1 = (i - 2); i1 < (i + 5); i1++){
        fprintf(stderr, "%llu: \"%s\"\n", i1, t.text[i1]);
      }
      exit(EXIT_FAILURE);
    }
    #endif

    //Get opcode from memonic
    u32 opcode = name_to_instr(t.text[i]);
    
    //remove memonic
    cstr tmp = cstrdup_stack(t.text[i]);
    if(strchr(tmp, ' '))
      tmp = strchr(tmp,  ' ') + 1; //Advance pointer to after the memonic and the spacebar after it
    else
      tmp = "0";
    
    u32 dest, a, b, c, d, e, f;
    
    //If the assembly does not need all of the arguments then sscanf will not set a,b,c so it is safe to use. 
    //All varibles are set to 0 which removes and possiblity that it will overrite any bits if set is used
    sscanf(tmp, "%u %u %u %u %u %u %u", &dest, &a, &b, &c, &d, &e, &f);
    if(logging){
      printf("pc: %llX:  %s %s\n", i * 4 + start_addr, OPCODE_NAMES[opcode], tmp);
    }
    //When calling the function pointer, the function being called has no clue how many arguments are being passed in.
    //All argument registers are overwritten in this function and the called function has no way of telling if there is arguments set past it's
    //intended reach so this approach has no expliots avaible and condenses the code a lot.
    PUSH32(ASM_OPCODE_PTR[opcode](opcode, dest, a, b, c, d, e ,f));
  }
  
  vector_deconstruct(&tmp_asm.labels);
  vector_deconstruct(&tmp_asm.asm_global.globals);
  fu_free_text_file(t);
  return ret;
}

fu_BinFile INLINE assemble(char* __restrict__ file_path){
  fu_BinFile ret;
  fu_TextFile input = fu_load_text_file(file_path);
  ret = assemble_s(input);
  fu_free_text_file(input);
  return ret;
}
