#include "FileUtils.h"

#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//#include "gfxengine/gfxcommands.h"
#include "assmbler/asm.h"
#include "assmbler/linker.h"

#include "bus.h"
#include "opcode.h"




static void print_help(void){
	fprintf(stderr, "Usage\n\tppcm [ROM FILENAME | SOURCE FILENAME] [-l|-o FILENAME]\n");
}



char DrawFrame;
char logging = false;
char output = false;

char stop_exe = 0;
int main(int argc, char* argv[]){
  fu_BinFile rom;
  logging = false;
  if(argc > 1){
    if(strcmp(argv[1], "-l") == 0){
      printf("ROM_SIZE: %X\nRAM_SIZE: %X\nSTACK_SIZE: %X\nFBUFFER_SIZE: %X\n", ROM_SIZE, RAM_SIZE - STACK_SIZE, STACK_SIZE, FRAMEBUFFER_SIZE);
      logging = true;
      argv++;
      argc--;
    }
    if(strcmp(argv[1], "-o") == 0){
        output = true;
        argv++;
        argc--;
    }
   	//Disabled until I can create a proper disassembler
    #if 0
    if(strcmp(argv[1], "-d") == 0){
      argv++;
      argc--;
      if(argv[1] != NULL){
        fu_BinFile binary = fu_load_bin_file(argv[1]);
        fu_TextFile decoded_binary = decode_bin(binary);
        for(fu_index i = 0; i < decoded_binary.size; i++){
          puts(decoded_binary.text[i]);
        }
        fu_free_bin_file(binary);
        fu_free_text_file(decoded_binary);
        exit(EXIT_SUCCESS);
      }else{
        fprintf(stderr, "ERR: No ROM provided!\n");
        exit(EXIT_FAILURE);
      }
    }
    #endif
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
      exit(EXIT_FAILURE);
    }else{
      rom = link_asm(files_to_assemble);
    }
    
  }else{
    //Display error message because no rom or asm was specified
    print_help();
    return EXIT_FAILURE;
  }
  
  if(output){
    fu_save_bin_file("./a.bin", rom);
    fu_free_bin_file(rom);
    exit(EXIT_SUCCESS);
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
