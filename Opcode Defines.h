#pragma once
#include <string>
#include <vector>

enum OPCODES {
	nop,
	//B - Instructions
	b,
	bl,
	blt,
	ble,
	bgt,
	bge,
	bne,
	beq,
	blr,
//Memroy
	//L - Instructions
	la,
	li,
	lw,
	lb,
	lhw,
	//S - Insructions
	sw,
	shw,
	sb,
	//M -Instructions
	mr,
//
	//Math Instructions
	add,
	addi,
	addiu,
	sub,
	subi,
	subiu,
	mul,
	muli,
	div_,
	divi,
	inc,
	inci,
	dec,
	deci,
	//Rotate / Shift commands
	rotlw,
	rotlwi,
	rotrw,
	rotrwi,
	slw,
	srw,
	slb,
	srb,
	slhw,
	srhw,
	slwi,
	srwi,
	slbi,
	srbi,
	slhwi,
	srhwi,
	//Comparison Commands
	cmp,
	cmpi,
	cmpr,
	cmpri,
	or_,
	ori,
	and_,
	andi,
	xor_,
	//CPU SPECIFIC
	BRANCH,
	SYSCALL
};
//
struct opcode {
	OPCODES op = nop;
	unsigned int * args=nullptr;
	unsigned int len=0;
};

const std::vector<std::string> op_list = {
	"nop",
	//B - Instructions
	"b",
	"bl",	
	"blt",
	"ble",
	"bgt",
	"bge",
	"bne",
	"beq",
	"blr",
	//L - Instructions
	"la",
	"li",
	"lw",
	"lb",
	"lhw",
	//S - Insructions
	"sw",
	"shw",
	"sb",
	//M -Instructions
	"mr",
	//Math Instructions
	"add",
	"addi",
	"addiu",
	"sub",
	"subi",
	"subiu",
	"mul",
	"muli",
	"div",
	"divi",
	"inc",
	"inci",
	"dec",
	"deci",
	//Rotate / Shift commands
	"rotlw",
	"rotlwi",
	"rotrw",
	"rotrwi",
	"slw",
	"srw",
	"slb",
	"srb",
	"slhw",
	"srhw",
	"slwi",
	"srwi",
	"slbi",
	"srbi",
	"slhwi",
	"srhwi",
	//Comparison Commands
	"cmp",
	"cmpi",
	"cmpr",
	"cmpri",
	"or",
	"ori",
	"and",
	"andi",
	"xor"
	"gregergrnoituru3phvyyerjuhgi4glekjgoi",
	"syscall"
};

OPCODES get_op(std::string str) {
	OPCODES ret = nop;
	if (str.size() > 2) {
		if (str[0] == 's' && str[1] == 'y') {
			return SYSCALL;
		}
	}
	for (unsigned int i = 0; i < op_list.size(); i++) {
		if (str == op_list[i]) {
			return OPCODES(i);
		}
	}

	if (str != "nop") {
		if (str == "syscall")
			return SYSCALL;
		return BRANCH;
	}
	return ret;
}
//