#include "FileUtils.h"

#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//#include "gfxengine/gfxcommands.h"
#include "assmbler/asm.h"
#include "assmbler/linker.h"

#include "bus.h"


fu_BinFile get_program(); //testprog.c
char DrawFrame;
char logging = false;
char output = false;

char stop_exe = 0;
int main(int argc, char* argv[]){
  fu_BinFile rom;
  logging = false;
  if(argc > 1){
    if(strcmp(argv[1], "-l") == 0){
      printf("ROM_SIZE: %X\nRAM_SIZE: %X\nSTACK_SIZE: %X\n", ROM_SIZE, RAM_SIZE - STACK_SIZE, STACK_SIZE);
      logging = true;
      argv++;
      argc--;
    }
    if(strcmp(argv[1], "-o") == 0){
        output = true;
        argv++;
        argc--;
    }
    if(strstr(argv[1], ".bin")){
      rom = fu_load_bin_file(argv[1]);
      goto main_program;
    }

    bool filesFound = false;
    fu_List files_to_assemble = fu_alloc_list(1);
    for(int i = 1; i < argc; i++){
      if(argv[i] == NULL) break; //Error checking
      if(!filesFound){ //If we have not used up the space that was allocated a few lines earlier then lets use that isntead of using realloc
        files_to_assemble.text[0] = strdup(argv[i]);
        filesFound = true;
      } else {
        fu_append_item(&files_to_assemble, strdup(argv[i]));
      }
    }
      
    if(!filesFound){
      fprintf(stderr, "ERR: The files you entered are not postfixed with the .s extension!\n");
      exit(-1);
    }else{
      rom = link_asm(files_to_assemble);
      if(rom.bin == NULL || rom.size == 0){
        fprintf(stderr, "ERR: There was no lines of code to assemble!\n");
        exit(EXIT_FAILURE);
      }
    }
    
  }else{
    //Get a built in program seeing as no program was given
    logging = true;
    rom = get_program();
    puts("RUNNING DEFAULT PROGRAM");
  }
  
  if(output){
    fu_save_bin_file("./a.bin", rom);
  }
  
  main_program:
  //gameengine();
  //sleep(10);
  load_rom(rom.bin, rom.size);
  
  
  while(!stop_exe){
    step();
    if(logging){
      print_cur();
      getc(stdin);
    }
    //system("clear");
    //print_cur();
    //DrawScreen();
    //usleep(2);
    //getc(stdin);
  }
  getc(stdin);
  return 0;
}
