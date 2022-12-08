# PowerPCM

A PowerPC emulator/assembler complete with a somewhat complete implementation of clib. In the future I might be able to add support for other RISC-V systems like MIPS and ARM because the syntax is so similar.

## How to Use ...

Simply pass in files that you want to run into the list of arguments. The program will determine if you are trying to run a binary or assemble a source file dependent on the file extension.

- **mySourceFile.s**  = Run Assembler and then Execute in emulator

- **myBinaryFile.bin** = Run Emulator

This means that if you pass in a source file it can be run as a executable. This is somewhat like python where you can just do "python myProgram.py". In the same way this is how you would run your program

```bash
PowerPCM mySourceFile.s
```

If you want to output a binary that can be run later then you can use the '-o' option

```bash
PowerPCM -o mySourceFile.s
```

This will create a binary called 'a.bin' in the current directory that is an assembled version of that source file. This file can be run without the need for the original source file.

```bash
PowerPCM a.bin
```

If you want to have a multi-file project you must specify all the files you need in the arguments. This is how you are able to use the clib I have provided. Because I did not make a proper linker, all labels will be global so that is just something to note.

```bash
PowerPCM -o mySourceFile.s ./asm_files/stdlib.s
```

To get access to functions such as malloc, puts, and free, make sure to include stdlib.s in the function arguments

## Windows Users
It is possible to get PowerPCM to run on windows. This is done by using mingw and using 
```bash
make UNAME=WIN32
```
to build it.

You can find a binary .exe release in the releases

## IMPORTANT DIFFERENCES TO POWERPC !!!!

I moved the stack pointer to r0 because it just made more sense. If you feel like you can not remember that change, just use 'sp' instead of 'r1' like so

```nasm
subi r0, r0, 8   --->>     subi sp, sp, 8
```

The other issue is that I changed some of the memonics. So for example ...

**lwz**  -->>  **lw**

**stw** -->> **sw**

Just check opcode.h for a list of memonic names. Eventually I will support the standard memonic names once I implement aliases for the memonic names.



## Debugging your PowerPC program

If you need to debug your program, you can step through the code using the -l option like so ...

```bash
PowerPCM -l mySourceFile.s
```

This is the output that you can expect

```nasm
SP0:	4E20	LI R2, 80062
R1:	    0   	SW R3, 0(R2)
R2:	    4E2A	LI R3, 80
R3:	    0	    LI R2, 80066
R4:	    1	    SW R3, 0(R2)
R5:	    4E 	    LI R6, 9
R6:  	9	    MUL R3, R3, R6
R7:	    0	    ADD R3, R3, R0
R8: 	0	    ADDI R3, R3, 1
R9:	    0	    LI R2, 80058
R10:	0	    SW R3, 0(R2)
R11:	0	    LI R3, 50000
R12:	0	    SUB R3, R3, R0
R13:	0	    SUBI R3, R3, 1000
R14:	0	    LI R2, 80070
R15:	0	    SW R3, 0(R2)
R16:	0	    LI R2, 80066
R17:	0	    LW R5, 0(R2)
R18:	0	    SUBI R5, R5, 1
R19:	0	    LI R3, 0
R20:	0	    LI R4, 1
R21:	0	    LI R2, 80062
R22:	0	    LW R2, 0(R2)
R23:	0	    SW R3, 0(R2)
R24:	0	    SW R3, 4(R2)
R25:	0	    SB R4, 8(R2)
R26:	0	    ADD R2, R2, R6
R27:	0	    SUBI R5, R5, 1
R28:	0	    CMPI R5, 0
R29:	0	    BNE 2260
R30:	0	    SW R3, 0(R2)
R31:	0	    SW R3, 4(R2)
PC:	8DC
LR:	0

```

On the left side you can see all of the registers and their values. On the right is an execution log of the last 32 instructions. The current instruction in the bottom most line. You will see the current line marked in orange in the real program.



Because there is no debugger for this application I would use GDB for more complex issues such as adding breakpoints to your program. This can be done in gdb like so ...

```bash
(gdb) watch cpu.pc
Hardware watchpoint 1: cpu.pc
(gdb) cond 1 cpu.pc == <FUNCTION_LOCATION>
```

This does mean that you will need to compile a debug version of the program to do this.



## Compiling

Just simple make. You can do either

- make release

- make debug



## Issues and Future Plans

---

- Linker simply copies and pastes all of the files that you list into one single file and then compiles it. This means that all labels are global. Making a real linker is hard. You shouldn't be using the same names for labels so this shouldn't be a huge problem

- Neagitive numbers are not supported. Subtraction still works though. If you subtract  from zero that will give you a negative number if that is absolutely needed. I plan to fix this as fast as possible.



TODO:

- [ ] Proper Linker

- [ ] '.include' support

- [ ] implement floating point instructions

- [ ] Implement CTR register

- [ ] Implement all other instructions

- [ ] Allow aliases for memonics

- [ ] Implement carry bit

- [ ] Implement negative numbers

- [ ] Add MIPS and ARM mode
