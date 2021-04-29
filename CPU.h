#pragma once
#include "Opcode Defines.h"
#include "string_tools.h"
#include "Assembler.h"

#include <Windows.h>

#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

olc::PixelGameEngine * _pge = nullptr;

enum COMP {
	EQ  = 0b0001,
	L   = 0b0010,
	G   = 0b0100,
	NEQ = 0b1000
};

void func_nop(void* ptr) {}

void func_b(void* ptr);
void func_bl(void* ptr);
void func_blr(void* ptr);
void func_bne(void* ptr);
void func_beq(void* ptr);
void func_blt(void* ptr);
void func_ble(void* ptr);
void func_bgt(void* ptr);
void func_bge(void* ptr);
void func_addi(void* ptr);
void func_add(void* ptr);
void func_li(void* ptr);
void func_lw(void* ptr);
void func_lb(void* ptr);
void func_mul(void* ptr);
void func_muli(void* ptr);
void func_sw(void* ptr);
void func_sb(void* ptr);
void func_mr(void* ptr);
void func_cmp(void* ptr);
void func_cmpi(void* ptr);
void func_syscall(void* ptr);
void func_BRANCH(void* ptr);

void (*micode[128])(void* ptr);

class CPU {
public:
	CPU() {
		std::fill(r,r + 32, 0);
		r[1] = 1024 * 4;
		std::fill(micode, micode + 128, func_nop);
		micode[b] = func_b;
		micode[bl] = func_bl;
		micode[beq] = func_beq;
		micode[bne] = func_bne;
		micode[blt] = func_blt;
		micode[ble] = func_ble;
		micode[bgt] = func_bgt;
		micode[bge] = func_bge;
		micode[blr] = func_blr;
		micode[addi] = func_addi;
		micode[mul] = func_mul;
		micode[muli] = func_muli;
		micode[add] = func_add;
		micode[li] = func_li;
		micode[lw] = func_lw;
		micode[lb] = func_lb;
		micode[sw] = func_sw;
		micode[sb] = func_sb;
		micode[mr] = func_mr;
		micode[cmp] = func_cmp;
		micode[cmpi] = func_cmpi;
		micode[SYSCALL] = func_syscall;
		micode[BRANCH] = func_BRANCH;
	}

public:
	program rdram;
public:
	unsigned int pc = 0;
	unsigned int lr = -1;
	unsigned r[32];
	unsigned int c = 0;
	unsigned char* mem = new unsigned char[1024 * 4];
	bool prog_comp = false;
	std::vector<unsigned int> poss;
public:
	void load_program(program prg) {
		rdram = prg;
		pc = prg.pgog_beg;
		std::fstream ofile("./out.bin", std::ios::out);
		ofile.write((char*)prg.db, 512);
		for (unsigned int i = 0; i < 512; i++) {
			mem[i] = (unsigned char)prg.db[i];
		}
	}

	void step() {
		
		prog_comp = true;
		if (pc >= rdram.bin.size()) return;
		prog_comp = false;
		void* args = nullptr;
		if (rdram.bin[pc].len != 0) {
			args = (void*)rdram.bin[pc].args;
		}
		//system("cls");
		//printf("~~~~~~~~~~~~~\n");

#if 1
		for (unsigned int i = 0; i < 32; i++) {
			//printf("r%i: \"%i\"\n", i, r[i]);
		}
#else
		printf("%i\n",pc);
#endif
		micode[rdram.bin[pc].op](args);
		
		pc++;
	}

}cpu;


/*
enum SYSCALL_ID {
	STRING,
	INT,
	FLOAT
};*/

#ifdef DEBUG
void print_isntr(std::string op_name, std::string format, int arg1 = -1, int arg2 =-1, int arg3 = -1) {
	std::string out_str = "PC: %i = %s "+ format;
	if (arg3 != -1) {
		printf(out_str.c_str(), cpu.pc, op_name.c_str(), arg1, arg2, arg3);
		return;
	}
	if (arg2 != -1) {
		printf(out_str.c_str(), cpu.pc, op_name.c_str(),arg1,arg2);
		return;
	}
	if (arg1 != -1) {
		printf(out_str.c_str(), cpu.pc, op_name.c_str(), arg1);
		return;
	}
	printf(out_str.c_str(), cpu.pc, op_name.c_str());
}
#else
void print_isntr(std::string op_name, std::string format, unsigned int arg1 = -1, unsigned int arg2 = -1, unsigned int arg3 = -1) {

}
#endif


void func_syscall(void* ptr) {

	void (*calls[8])() {
		[]() {//Str plus str 0
			printf(((char*)cpu.mem + cpu.r[4]), (char*)(void*)&cpu.r[5]);
		},
		[]() {//str plus int
		//	printf(((char*)cpu.mem + cpu.r[4])); 1
			printf(((char*)cpu.mem + cpu.r[4]), cpu.r[5]);
		},
		[]() {//str plus float 2
			printf(((char*)cpu.mem + cpu.r[4]), *(float*)(void*)cpu.r[5]);
		},
		[]() {//str 3
			printf((char*)cpu.mem + cpu.r[4]);
		},
		[]() { // 4
			scanf_s(((char*)cpu.mem + cpu.r[4]), (char*)(void*)&cpu.r[5]);
		},
		[]() {//5
			scanf_s("%i", (int*)(cpu.mem + cpu.r[4]));
		},
		[]() {//6
			scanf_s("%f", (float*)(void*)&cpu.r[5]);
		},
		[]() {
			cpu.r[4] = rand() % cpu.r[4];
		},
	};

	calls[cpu.r[3]]();
	print_isntr("syscall", "%i", cpu.r[3]);
}

//Function Defines
void func_b(void * ptr) {
	cpu.pc = *(int*)(void*)ptr;
	print_isntr("bl", "%i", *(int*)(void*)ptr);
}
void func_bl(void* ptr) {
	cpu.lr = cpu.pc;
	cpu.pc = *(int*)(void*)ptr;
	print_isntr("bl", "%i", *(int*)(void*)ptr);
}
void func_blr(void* ptr) {
	cpu.pc = cpu.lr;
	print_isntr("blr", "");
}
void func_bne(void* ptr) {
	int args = *(int*)(void*)ptr;
	if (cpu.c & NEQ)
		cpu.pc = args;
	print_isntr("bne", "%i", args);
}
void func_beq(void* ptr) {
	int args = *(int*)(void*)ptr;
	if (cpu.c & EQ)
		cpu.pc = args;
	print_isntr("beq", "%i", args);
}
void func_blt (void* ptr) {
	int args = *(int*)(void*)ptr;
	if (cpu.c & L)
		cpu.pc = args;
	print_isntr("blt", "%i", args);
}
void func_ble(void* ptr) {
	int args = *(int*)(void*)ptr;
	if (cpu.c & (EQ | L))
		cpu.pc = args;
	print_isntr("ble", "%i", args);
}
void func_bgt(void* ptr) {
	int args = *(int*)(void*)ptr;
	if (cpu.c & G)
		cpu.pc = args;
	print_isntr("bgt", "%i", args);
}
void func_bge(void* ptr) {
	int args = *(int*)(void*)ptr;
	if (cpu.c & (EQ | G))
		cpu.pc = args;
	print_isntr("bge", "%i", args);
}
void func_addi(void* ptr) {
	int* arr = (int*)ptr;
	cpu.r[arr[0]] = cpu.r[arr[1]] + arr[2];
	print_isntr("addi", "r%i, r%i, %i", arr[0], arr[1], arr[2]);
}
void func_add(void* ptr) {
	int* arr = (int*)ptr;
	cpu.r[arr[0]] = cpu.r[arr[1]] + cpu.r[arr[2]];
	print_isntr("add", "r%i, r%i, r%i", arr[0], arr[1], arr[2]);
}
void func_li(void* ptr) {
	int* arr = (int*)ptr;
	cpu.r[arr[0]] = arr[1];
	print_isntr("li", "r%i, %i", arr[0], arr[1]);
}
void func_lw(void* ptr) {
	int* args = (int*)(void*)ptr;
	int* int_mem = (int*)(void*)cpu.mem;
	int index = (args[1] + cpu.r[args[2]])/sizeof(int);
	cpu.r[args[0]] = int_mem[index];
	print_isntr("lw", "r%i, %i(r%i)", args[0], args[1], args[2]);
}
void func_lb(void* ptr) {
	int* args = (int*)(void*)ptr;
	cpu.r[args[0]] = cpu.mem[cpu.r[args[2]] + args[1]];
	print_isntr("lb", "r%i, %i(r%i)", args[0], args[1], args[2]);
}
void func_mul(void* ptr) {
	int* arr = (int*)ptr;
	cpu.r[arr[0]] = cpu.r[arr[1]] * cpu.r[arr[2]];
	print_isntr("mul", "r%i, r%i, r%i", arr[0], arr[1], arr[2]);
}
void func_muli(void* ptr) {
	int* arr = (int*)ptr;
	cpu.r[arr[0]] = cpu.r[arr[1]] * arr[2];
	print_isntr("muli", "r%i, r%i, %i", arr[0], arr[1], arr[2]);
}
void func_sw(void* ptr) {
	int* arr = (int*)ptr;
	int val = arr[0];
	char * save = (char*)(void*)&val;
	int offset = arr[1];
	int base = arr[2];

	int index = offset + cpu.r[base];
	cpu.poss.push_back(index);
	cpu.mem[index + 0] = save[0];
	cpu.mem[index + 1] = save[1];
	cpu.mem[index + 2] = save[2];
	cpu.mem[index + 3] = save[3];
	print_isntr("sw", "r%i, %i(r%i)", arr[0], arr[1], arr[2]);
}
void func_sb(void* ptr) {
	int* arr = (int*)ptr;
#ifdef DEBUG
	printf("%c\n", (char)cpu.r[arr[0]]);
#endif
	cpu.mem[arr[1] + cpu.r[arr[2]]] = cpu.r[arr[0]];
	print_isntr("sb", "r%i, %i(r%i)", arr[0], arr[1], arr[2]);
}
void func_mr(void* ptr) {
	int* args = (int*)(void*)ptr;
	cpu.r[args[0]] = cpu.r[args[1]];
	print_isntr("mr", "r%i, r%i", args[0], args[1]);
}
void func_cmp(void* ptr) {
	int* args = (int*)(void*)ptr;
	int num1 = cpu.r[args[0]];
	int num2 = cpu.r[args[1]];
	cpu.c = 0;
	cpu.c |= (EQ * (num1 == num2));
	cpu.c |= (L * (num1 < num2));
	cpu.c |= (G * (num1 > num2));
	cpu.c |= (NEQ * (num1 != num2));
	print_isntr("cmp", "r%i, r%i",num1,num2);
}
void func_cmpi(void* ptr) {
	int* args = (int*)(void*)ptr;
	int num1 = cpu.r[args[0]];
	int num2 = args[1];
	cpu.c = 0;
	cpu.c |= (EQ * (num1 == num2));
	cpu.c |= (L * (num1 < num2));
	cpu.c |= (G * (num1 > num2));
	cpu.c |= (NEQ * (num1 != num2));
	print_isntr("cmp", "r%i, r%i", num1, num2);
}
void func_BRANCH(void* ptr) {
	print_isntr("BRANCH HEADER / NOP", "");
}