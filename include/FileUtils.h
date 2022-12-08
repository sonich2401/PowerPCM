/*RenDev#2616 File Utilites v1.3
    Copyright (C) 2021 RenDev

    Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.
    Permission is also granted to not credit the author in any way as long as you do not take credit
    for this piece of software.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT,
    OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
    DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
    ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
    IF YOU DO THE HOKEY POKEY YOU TURN YOURSELF AROUND.

Contact me at vgngamingnetwork@gmail.com if you need to contact me about this licence*/

/***********************    README ********************************

  What is this?:
    This is a single file solution to all of your file accessing needs.

  Why C?
    Because it can work in both C++ and C code.

  How do I use it?
    Define FU_IMP in ONLY one .c/.cpp file.
    I would recommend that you define it in a separate .c/.cpp file so that if you make changes
    to your other .c files it will not have to recompile this.
    If you do not care about compile times, staticly compile this file with the settings flags

    If you want better performance in C++ then compile this file as C code. This header
    will automaticly make it compatible with C++


    !!! NOTICE: MAKE SURE TO LOOK AT THE AVAILABLE SETTINGS LOCATED BELOW THIS README!!!
  How does memory management work?
    You will have to remember to free up any lists or bin files
    that you have laying around that you are no longer using.
    You can do this by doing
      fu_free_<NAME OF TYPE>(object);

  How do Lists work?
    Lists, Text, and Bin files all use the same struct.
    There are 2 members
      text / bin
      size
    text / bin is a pointer to raw memory
    size is the amount of elements stored at that raw memory.
    for text it will tell you how many lines there are and for
    bin it will tell you how many bytes are in the file.

  Can I use this in multiple files?
    Yes. Because it is a single file solution it can only be defined once but you can include
    this as a header anywhere in your programs.

  Am I allowed to use this without crediting you?
    YES
    Open source all the way.
    You do not have to credit me but I would appreciate it :)
    If you would like AND ONLY if you want to credit me, either
      Link my GH: github.com/sonich2401
      Link my Discord: RenDev#2616
        or
      Link my email: vgngamingnetwork@gmail.com
v1.3
  - Added more DEBUG flags and fixed some OOB memory access
  - You can now recursively search for files and folders
v1.2
  - Added text file append
  - Added text line insert
  - Added text line delete
  - Added better header definitions
v1.1
  - Added wildcard functionality
  - Added file find functionality (working on a GNU backend soon)
  - Added way to staticly compile the code #define FU_STATIC_COMPILE
  - Added access time to FileStats
  - Added file extension to FileStats
  - Renamed all structs to include the 'fu_' prefix
  - FileStats now includes file type
  - Faster file time retrieval
  - General performance increases + less RAM used
  - 0 Warnings with -Wall using both gcc and g++ (except for -Wunused-function)
  - Made using '__restrict__' optional with FU_NO_RESTRICT
  - Moved C++ only features to default off.

v1.0
  - Inital release


*********************************** END OF README ***********************************/



//$$$$ DEFINE SETTINGS HERE $$$$
//Uncomment to force struct functions (decreases performance (EXPERIMENTAL C++ ONLY))
  //#define FU_USE_STRUCT_FUNCTIONS
//Uncomment line below to disable restrict (helps compiling on old C++ compilers but helps performance if you do not disable restrict)
  //#define FU_NO_RESTRICT
//Uncomment to enable debugging messages
  #define FU_DEBUG true
//Uncomment to staticly compile all functions which allows you not to use #define FU_IMP
  //#define FU_STATIC_COMPILE





//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#ifndef __cplusplus
  #define FU_NULL NULL
  #include <stdbool.h> //bool, true, false
#else
  #define FU_NULL nullptr
#endif

//[Use/Do not use] restrict to increase performance (might break old C++ compilers)
#ifdef FU_NO_RESTRICT
  #define FU_RESTRICT
#else
  #define FU_RESTRICT __restrict__
#endif

#define FU_COMPILE_MODE

//Staticly compile the code
#ifdef FU_STATIC_COMPILE
  #undef FU_COMPILE_MODE
  #define FU_COMPILE_MODE static
  #ifndef FU_IMP //Define the implement flag if it hasn't already
    #define FU_IMP
  #endif
#endif

#ifdef FU_INLINE_COMPILE
  #undef FU_COMPILE_MODE
  #define FU_COMPILE_MODE inline
  #ifndef FU_IMP //Define the implement flag if it hasn't already
    #define FU_IMP
  #endif
#endif

#ifndef PATH_MAX
#define PATH_MAX 512
#endif


#ifdef WIN32
#define lstat stat
#define srandom srand
#define random rand
#endif

#ifdef __cplusplus
extern "C" {
#endif

//A standard type that is used to index things like lists
typedef unsigned long long int fu_index;
//A standard type for a string
typedef char* fu_string;
//A standard type for binary
typedef fu_string fu_hexarray;

//A special struct to hold a bin file
//fu_List.bin is an array of bytes.
//You can access the data by doing myBinFile.bin[index];
//You can get the number of bytes in the bin file by doing myBinFile.size;
#ifndef FU_USE_STRUCT_FUNCTIONS
typedef struct{
  fu_index size;
  fu_hexarray bin;
}fu_BinFile;
#else
struct fu_BinFile{
  fu_BinFile();
  fu_BinFile(const fu_string path);
  fu_BinFile(fu_index bsize, fu_hexarray buffer);

  fu_index size;
  fu_hexarray bin;

  fu_index get_hash();
  char operator[](fu_index index);


  ~fu_BinFile();
};
#endif

//A special struct to hold a list of items. Namely text.
//fu_List.text is a 2D array of text that stores the lines of the list.
//You can access the data by doing myList.text[index];
//You can get the number of items in the list by doing myList.size;
#ifndef FU_USE_STRUCT_FUNCTIONS
  typedef struct {
    fu_index size;
    fu_string* text;
  }fu_List;
#else
  struct fu_List {
    fu_List();
    fu_List(const fu_string path);
    fu_List(fu_index lines, fu_string* items);

    fu_index size;
    fu_string* text;

    void load_text_file(const fu_string path);
    fu_BinFile to_bin();
    fu_stringoperator[](fu_index index);

    ~fu_List();
  };
#endif


//A special struct to hold a list of items. Namely text.
//fu_List.text is a 2D array of text that stores the lines of the list.
//You can access the data by doing myList.text[index];
//You can get the number of items in the list by doing myList.size;
typedef fu_List fu_TextFile;


//A timestamp that is derived from a file
typedef struct{
  char hour;
  char minute;
  char second;
  char day;
  unsigned char month;
  unsigned short year;
}fu_Timestamp;

//This union/struct will allow you to get the rwx status of a file. The names are self explainitory
typedef union{
  struct{
    bool exists : 1;
    bool not_accessible : 1;
    bool readable : 1;
    bool writeable : 1;
    bool read_only : 1;
    bool executable : 1;
  };
  unsigned char chr_form;
}fu_FilePerms;

typedef enum{
  FU_UNKOWN = 0,
  FU_BLOCK_DEVICE = 1,
  FU_CHARACTER_DEVICE = 2,
  FU_DIRECTORY = 3,
  FU_FIFO = 4,
  FU_PIPE = 4,
  FU_SYSLINK = 5,
  FU_SYSTEM_LINK = 5,
  FU_FILE = 6,
  FU_SOCKET = 7
}fu_FileType;

//This struct will contain all information for a given file.
//It includes modified and created time, permissions, file name, and the size of the file

#ifndef FU_USE_STRUCT_FUNCTIONS
typedef struct {
#else
struct fu_FileStats{
#endif
  fu_Timestamp modified;
  fu_Timestamp created;
  fu_Timestamp accessed;
  fu_string file_name;
  fu_string file_extension;
  fu_FileType type;
  fu_FilePerms perms;

  unsigned long bytes;

#ifdef FU_USE_STRUCT_FUNCTIONS
  ~fu_FileStats();
};
#else
}fu_FileStats;
#endif


//This function will return a list of all files and folders contained in the directory given.
//Use the `show_hidden` flag to see hidden files
FU_COMPILE_MODE fu_List fu_get_dir_contents(fu_string FU_RESTRICT folder, bool full_path, bool show_hidden, bool recursive);
//This function will return a list of all files contained in the directory given.
//Use the `show_hidden` flag to see hidden files
FU_COMPILE_MODE fu_List fu_get_dir_files(fu_string FU_RESTRICT folder, bool full_path, bool show_hidden, bool recursive);
//This function will return a list of all folders contained in the directory given.
//Use the `show_hidden` flag to see hidden files
FU_COMPILE_MODE fu_List fu_get_dir_folders(fu_string FU_RESTRICT folder, bool full_path, bool show_hidden, bool recursive);

//Get a struct containing all info about a file. See the FileStats struct for more info
FU_COMPILE_MODE fu_FileStats fu_get_file_info(const fu_string FU_RESTRICT file_path);
//Get the type of file located at file_path. Returns a fu_FileType enum value
FU_COMPILE_MODE fu_FileType fu_get_file_type(const fu_string FU_RESTRICT file_path);
//Get a timestamp for when a given file was created
FU_COMPILE_MODE fu_Timestamp fu_get_file_created_time(const fu_string FU_RESTRICT file_path);
//Get a timestamp for when a given file was modified
FU_COMPILE_MODE fu_Timestamp fu_get_file_modified_time(const fu_string FU_RESTRICT file_path);
//Get a timestamp for when a given file was modified
FU_COMPILE_MODE fu_Timestamp fu_get_file_accessed_time(const fu_string FU_RESTRICT file_path);
//Get the size of a given file
FU_COMPILE_MODE fu_index fu_get_file_size(const fu_string FU_RESTRICT file_path);
//Get file / folder permissions
FU_COMPILE_MODE fu_FilePerms fu_get_file_perms(const fu_string FU_RESTRICT file_path);

//Load a binary file into a fu_List struct. See fu_List for more info on how to use
FU_COMPILE_MODE fu_BinFile fu_load_bin_file(const fu_string FU_RESTRICT file_path);
//Load a text file into a fu_List struct. See fu_List for more info on how to use
FU_COMPILE_MODE fu_TextFile fu_load_text_file(const fu_string FU_RESTRICT file_path);

//Save a binary file to the disk.
FU_COMPILE_MODE void fu_save_bin_file(const fu_string FU_RESTRICT file_path, fu_BinFile bin);
//Save a text file to the disk.
FU_COMPILE_MODE void fu_save_text_file(const fu_string FU_RESTRICT file_path, fu_TextFile text);

//Allocate lines for a text file
FU_COMPILE_MODE fu_TextFile fu_alloc_text_file(fu_index line_count);
//Allocate lines for a List
FU_COMPILE_MODE fu_List fu_alloc_list(fu_index element_count);
//Allocate lines for a bin file
FU_COMPILE_MODE fu_BinFile fu_alloc_bin_file(fu_index byte_count);

//Duplicate contents from src to dest
FU_COMPILE_MODE fu_BinFile* fu_memcpy_bin_file(fu_BinFile * dest, fu_BinFile* FU_RESTRICT src);
//Duplicate contents from src to dest
FU_COMPILE_MODE fu_TextFile* fu_memcpy_text_file(fu_TextFile * dest, fu_TextFile* FU_RESTRICT src);
//Duplicate contents from src to dest
FU_COMPILE_MODE fu_List* fu_memcpy_list(fu_List * dest, fu_List* FU_RESTRICT src);

//Duplicate contents from src to dest
FU_COMPILE_MODE fu_BinFile fu_create_bin_copy(fu_BinFile src);
//Duplicate contents from src to dest
FU_COMPILE_MODE fu_TextFile fu_create_text_copy(fu_TextFile src);
//Duplicate contents from src to dest
FU_COMPILE_MODE fu_List fu_create_list_copy(fu_List src);

//Join 2 Lists together and returns a new list contaning a + b
FU_COMPILE_MODE fu_List fu_join_list_copy(fu_List a, fu_List b);
//Join 2 bin files together and returns a new bin contaning a + b
FU_COMPILE_MODE fu_BinFile fu_join_bin_copy(fu_BinFile a, fu_BinFile b);

//Join 2 Lists together and stores resulting data in 'a'
FU_COMPILE_MODE fu_List fu_join_list(fu_List * a, fu_List b);
//Join 2 bin files together and stores resulting data in 'a'
FU_COMPILE_MODE fu_BinFile fu_join_bin(fu_BinFile * a, fu_BinFile b);

//Append text to the end of a text file
FU_COMPILE_MODE fu_TextFile fu_append_text(fu_TextFile * ptr, fu_string FU_RESTRICT str);
//Append text to the end of a list
FU_COMPILE_MODE fu_TextFile fu_append_item(fu_List * ptr, fu_string FU_RESTRICT str);

//Insert text line at line 'i'
FU_COMPILE_MODE fu_TextFile fu_insert_text(fu_TextFile * ptr, fu_index i, fu_string FU_RESTRICT str);
//Insert text line at line 'i'
FU_COMPILE_MODE fu_TextFile fu_insert_item(fu_List * ptr, fu_index i, fu_string FU_RESTRICT str);

//Deletes text line at line 'i'
FU_COMPILE_MODE fu_TextFile fu_delete_text(fu_TextFile * ptr, fu_index i);
//Deletes text line at line 'i'
FU_COMPILE_MODE fu_TextFile fu_delete_item(fu_List * ptr, fu_index i);

//Gets a new struct that contants elements 'b' to 'e'
FU_COMPILE_MODE fu_TextFile fu_sub_text(fu_TextFile file, fu_index b, fu_index e);
//Gets a new struct that contants elements 'b' to 'e'
FU_COMPILE_MODE fu_List fu_sub_list(fu_List list, fu_index b, fu_index e);
//Gets a new struct that contants elements 'b' to 'e'
FU_COMPILE_MODE fu_BinFile fu_sub_bin(fu_BinFile file, fu_index b, fu_index e);

//Get a hash for a bin file
FU_COMPILE_MODE fu_index fu_get_hash(fu_BinFile file);
//Convert a fu_TextFile into a fu_BinFile. This will allocate more memory so make sure to free this
FU_COMPILE_MODE fu_BinFile fu_to_bin(fu_TextFile text_file);

//Free memory inside of a fu_List object
FU_COMPILE_MODE void fu_free_list(fu_List list);
//Free memory inside of a fu_TextFile object
FU_COMPILE_MODE void fu_free_text_file(fu_TextFile txt_file);
//Free memory inside of a fu_BinFile object
FU_COMPILE_MODE void fu_free_bin_file(fu_BinFile bin_file);
//Free memory inside of a FileStats object
FU_COMPILE_MODE void fu_free_FileStats(fu_FileStats filestat);


//Convert a Windows text file to a UNIX text file.
FU_COMPILE_MODE void fu_text_file_convert_unix(fu_TextFile txt_file);
//Update the modified time of a file and create it if it does not exist
FU_COMPILE_MODE void fu_touch_file(const fu_string FU_RESTRICT file_path);
//Return a string containing the name of the user.
FU_COMPILE_MODE fu_string fu_get_current_user();

//Get list of all files with a given wildcard
FU_COMPILE_MODE fu_List fu_wildcard(fu_List path, const fu_string FU_RESTRICT format);
//Find files
FU_COMPILE_MODE fu_List fu_find(const fu_string FU_RESTRICT path, const fu_string FU_RESTRICT name, bool show_hidden, bool recursive);


#ifdef __cplusplus
}
#endif


#ifdef FU_IMP
#include <stdlib.h>
#include <string.h> //malloc(), strcpy(), free()
#include <time.h> //time struct, localtime()
#include <unistd.h> //access()
#include <dirent.h> //dirent, opendir(), closedir(), readdir()
#include <errno.h> //errno
#include <stdio.h> //perror(), printf(), fopen(), fseek(), fwrite(), ftell()
#include <sys/stat.h> //File stating, lstat()

#define FU_STRDUP(src) ((fu_string)strcpy(((fu_string)malloc(strlen(src) + 1)), ((fu_string)src)))

FU_COMPILE_MODE void fu_helper__init_timestamp(fu_Timestamp * FU_RESTRICT ts){
  ts->day = 0;
  ts->month = 0;
  ts->year = 0;
  ts->hour = 0;
  ts->minute = 0;
  ts->second = 0;
}

FU_COMPILE_MODE fu_Timestamp fu_helper__to_timestamp(struct timespec * FU_RESTRICT ts){
  fu_Timestamp ret;
  struct tm *conv_tm = (struct tm*)localtime(&ts->tv_sec);

  ret.second = conv_tm->tm_sec;
  ret.minute = conv_tm->tm_min;
  ret.hour = conv_tm->tm_hour;

  ret.day = conv_tm->tm_mday;
  ret.month = conv_tm->tm_mon;
  ret.year = conv_tm->tm_year + 1900;
  return ret;
}

FU_COMPILE_MODE fu_FileType fu_helper__get_file_type(struct stat * FU_RESTRICT st){
  switch(st->st_mode & S_IFMT){
    case S_IFBLK:
      return FU_BLOCK_DEVICE;
    case S_IFCHR:
      return FU_CHARACTER_DEVICE;
    case S_IFDIR:
      return FU_DIRECTORY;
    case S_IFIFO:
      return FU_FIFO;
	#ifndef WIN32
    case S_IFLNK:
      return FU_SYSLINK;
	  case S_IFSOCK:
      return FU_SOCKET;
	#endif
    case S_IFREG:
      return FU_FILE;

    default:
      return FU_UNKOWN;
  }

  return FU_UNKOWN;
}

FU_COMPILE_MODE void fu_helper__init_fstat(fu_FileStats * FU_RESTRICT fstat){
  fstat->bytes = 0;
  fu_helper__init_timestamp(&fstat->created);
  fu_helper__init_timestamp(&fstat->modified);
  fstat->type = FU_UNKOWN;
}

FU_COMPILE_MODE void fu_helper__set_timestamp(fu_Timestamp * FU_RESTRICT ts, int sec, int min, int h){
  ts->second = sec;
  ts->minute = min;
  ts->hour = h;
}

FU_COMPILE_MODE char fu_helper__isDir(struct dirent* FU_RESTRICT dir){
  //printf("TYPE = %i, TEST = %i, NAME = \"%s\", BOOL = %i\n", dir->d_type, DT_DIR, dir->d_name, dir->d_type == DT_DIR);
	struct stat sbuff;
  #ifndef WIN32
    if(dir->d_type == DT_UNKNOWN){
      lstat(dir->d_name, &sbuff);
      return S_ISDIR(sbuff.st_mode) && !S_ISLNK(sbuff.st_mode);   
    }
  #endif
   
  #ifndef WIN32
	  return dir->d_type == DT_DIR && dir->d_type != DT_LNK;
  #else
    return S_ISDIR(sbuff.st_mode);
  #endif
}

FU_COMPILE_MODE char fu_helper__isFile(struct dirent* FU_RESTRICT dir){
  struct stat sbuff;
  
  
  #ifndef WIN32
    lstat(dir->d_name, &sbuff);
    if(dir->d_type == DT_UNKNOWN){
      return S_ISREG(sbuff.st_mode) && !S_ISLNK(sbuff.st_mode);
    }
    if(S_ISLNK(sbuff.st_mode)){
      return 0;
    }
    return dir->d_type == DT_REG && dir->d_type != DT_LNK;
  #else
    stat(dir->d_name, &sbuff);
    return S_ISDIR(sbuff.st_mode);
  #endif
}


//Check and remove invalid characters
FU_COMPILE_MODE void fu_check_characters(char* FU_RESTRICT str){
  if(!str) return;
  fu_index i = 0;
  while(*str != 0){
    char og_ch = *str;
    i++;
    if(i >= 5000){ //Dont go over 5000
      return;
    }
    if(*str > 0 && *str <=31){
      *str = ' '; //
    }
    if(*str >= 128){
      *str = ' ';
    }
    if(*str != og_ch){
      abort();
    }
    str++;
  }
}

//If folder is true then include folders in the list
//If files is true then include files in the list
//if show_hidden is true then show the hidden files
FU_COMPILE_MODE fu_List fu_helper__get_dir_contents(fu_string FU_RESTRICT folder, bool folders, bool files, bool full_path, bool show_hidden, bool recursive){
  fu_string* ret = NULL;
  fu_check_characters(folder);
  fu_index folder_strlen = strnlen(folder, 4000);

  DIR * d = opendir(folder);
  struct dirent* dir;


  fu_index element_count = 0;
  if(d == NULL){
    #ifdef FU_DEBUG
      if(folder != NULL){
        fprintf(stderr, "WARNING: fu_helper__get_dir_contents() failed to stat file/directory path %s\n", folder);
      }else{
        fprintf(stderr, "WARNING: fu_helper__get_dir_contents() failed to stat file/directory path because the folder was a null pointer\n");
      }
    #endif
    fu_List err_ret;
    err_ret.text = FU_NULL;
    err_ret.size = 0;
    return err_ret;
  }

  while((dir = readdir(d)) != NULL){
    #ifdef FU_DEBUG
      if(folder_strlen > 4000){
        abort();
      }
    #endif
    //Get rid of "." and ".." directory entries
    fu_check_characters(dir->d_name);
    if(dir->d_name[0] == '.'){
      if(!show_hidden)
        continue;
      if(strlen(dir->d_name) == 1)
        continue;
      if(dir->d_name[1] == '.'){
        if(strlen(dir->d_name) == 2)
          continue;
      }
    }
    if(fu_helper__isDir(dir) & !folders){
      if(recursive){
        fu_List tmp_list;
        tmp_list.text = ret;
        tmp_list.size = element_count;
        
        char subfolder_path[strlen(dir->d_name) + folder_strlen + 2];
        sprintf(subfolder_path, "%s/%s", folder, dir->d_name);
        //printf("SUBPATH: \"%s\"\n", subfolder_path);
        fu_List subfolder_contents = fu_helper__get_dir_contents(subfolder_path, folders, files, full_path, show_hidden, recursive);
        if(subfolder_contents.size == 0){ //Avoid errors with fu_join_list
          continue;
        }
        if(ret != NULL){
          fu_join_list(&tmp_list, subfolder_contents);
          fu_free_list(subfolder_contents);
        } else {
          tmp_list = subfolder_contents; //RET was null so we instead use the newly allocated buffer as the list
        }
        
        ret = tmp_list.text;
        element_count = tmp_list.size;
        continue;
      }
      continue;
    }
    if(fu_helper__isFile(dir) & !files){
      continue;
    }

    if(ret == NULL){
      ret = (fu_string*)malloc(sizeof(fu_string) * 1);
      //for(int i = 0; i < 5; i++){
      //  ret[i] = FU_NULL;
      //}
    }else{
      /*if(cursor - 1 <= element_count){ //Are we running out of element spaces?
        ret = (fu_string*)realloc(ret, sizeof(fu_string) * ((element_count + 1) * 1.25)); // allocate more than what we need so that we do not have to realloc every loop
        #ifdef FU_DEBUG
          if(ret == FU_NULL){
            fprintf(stderr, "ERR: fu_helper__get_dir_contents(): Ran out of RAM!\n");
            abort();
          }
        #endif
        cursor = (element_count + 1) * 1.25;
        //for(int i = element_count; i <= cursor; i++){
        //  ret[i] = FU_NULL;
        //}
      }*/
      
      ret = (fu_string*)realloc(ret, sizeof(fu_string) * ((element_count + 1))); 
    }
    
    #ifdef FU_DEBUG
      //bool err_print = false;
      //if(ret[element_count] != NULL && ret[element_count] > 0x400){
      //  fprintf(stderr, "WARNING: fu_helper__get_dir_contents(): ret[%llu] has been overritten and was not null!\n", element_count);
      //  fprintf("OLD = \"%s\"\n", ret[element_count]);
        //err_print = true;
      //}
    #endif

    if(full_path){
      ret[element_count] = (fu_string)malloc(strlen(dir->d_name) + folder_strlen + 2);
      
      #ifdef FU_DEBUG
        if(ret[element_count] == NULL){
          fprintf(stderr, "ERR: fu_helper__get_dir_contents(): Out of RAM!\n");
          abort();
        }
      #endif
      sprintf(ret[element_count], "%s/%s", folder, dir->d_name);
    }else{
      ret[element_count] = (fu_string)malloc(strlen(dir->d_name) + 1);
      #ifdef FU_DEBUG
        if(ret[element_count] == NULL){
          fprintf(stderr, "ERR: fu_helper__get_dir_contents(): Out of RAM!\n");
          abort();
        }
      #endif
      strcpy(ret[element_count], dir->d_name);
    }
  
    element_count++;
  }

  closedir(d);
  

  

  fu_List return_struct;
  return_struct.size = (fu_index)element_count;
  return_struct.text = ret;
  
  /*if(recursive){
    for(fu_index i = 0; i < (fu_index)element_count; i++){
      fu_List tmp = fu_helper__get_dir_contents(return_struct.text[i], folders, files, full_path, show_hidden, recursive);
      fu_join_list(&return_struct, tmp);
      fu_free_list(tmp);
    }
  }*/
  return return_struct;
}

FU_COMPILE_MODE fu_List fu_get_dir_contents(fu_string FU_RESTRICT folder, bool full_path, bool show_hidden, bool recursive){
  return fu_helper__get_dir_contents(folder, 1, 1, full_path, show_hidden, recursive);
}

FU_COMPILE_MODE fu_List fu_get_dir_files(fu_string FU_RESTRICT folder, bool full_path, bool show_hidden, bool recursive){
  return fu_helper__get_dir_contents(folder, 0, 1, full_path, show_hidden, recursive);
}

FU_COMPILE_MODE fu_List fu_get_dir_folders(fu_string FU_RESTRICT folder, bool full_path, bool show_hidden, bool recursive){
  return fu_helper__get_dir_contents(folder, 1, 0, full_path, show_hidden, recursive);
}

FU_COMPILE_MODE void fu_free_list(fu_List list){
  for(fu_index i = 0; i < list.size; i++){
    free(list.text[i]);
  }
  free(list.text);
  list.text = NULL;
}

FU_COMPILE_MODE void fu_free_text_file(fu_TextFile txt_file){
  fu_free_list(txt_file);
}

FU_COMPILE_MODE void fu_free_bin_file(fu_BinFile bin_file){
  #ifdef FU_USE_STRUCT_FUNCTIONS
    if(bin_file.bin == nullptr)
      return;
  #endif
  free(bin_file.bin);
}


FU_COMPILE_MODE void fu_free_FileStats(fu_FileStats filestat){
  free(filestat.file_name);
}

FU_COMPILE_MODE fu_FileStats fu_get_file_info(const fu_string FU_RESTRICT file_path){
  struct stat futil__st;
  #ifndef WIN32
    lstat(file_path, &futil__st);
  #else
    stat(file_path, &futil__st);
  #endif

  fu_FileStats ret;
  ret.file_name = (fu_string)malloc(strlen(file_path));
  ret.file_name = strcpy(ret.file_name, file_path);

  fu_string last = FU_NULL;
  fu_string cur = ret.file_name;
  while((cur = strstr(cur, ".")) != FU_NULL){
    cur++;
    last = cur;
  };
  if(last != FU_NULL){
    cur = (fu_string)malloc(strlen(last) + 1);
    last = strcpy(cur, last);
    last = cur;
  }
  ret.file_extension = last;


  ret.bytes = futil__st.st_size;
  ret.type = fu_helper__get_file_type(&futil__st);
  #ifndef WIN32
    ret.modified = fu_helper__to_timestamp(&futil__st.st_mtim);
    ret.created = fu_helper__to_timestamp(&futil__st.st_ctim);
    ret.accessed = fu_helper__to_timestamp(&futil__st.st_atim);
  #else
    ret.modified = fu_helper__to_timestamp(&futil__st.st_mtime);
    ret.created = fu_helper__to_timestamp(&futil__st.st_ctime);
    ret.accessed = fu_helper__to_timestamp(&futil__st.st_atime);
  #endif
  return ret;
}

FU_COMPILE_MODE fu_Timestamp fu_get_file_created_time(const fu_string FU_RESTRICT file_path){
  struct stat tmp;
  lstat(file_path, &tmp);
  #ifndef WIN32
    return fu_helper__to_timestamp(&tmp.st_ctim);
  #else
    return fu_helper__to_timestamp(&tmp.st_ctime);
  #endif
}

FU_COMPILE_MODE fu_Timestamp fu_get_file_modified_time(const fu_string FU_RESTRICT file_path){
  struct stat tmp;
  lstat(file_path, &tmp);
  #ifndef WIN32
    return fu_helper__to_timestamp(&tmp.st_mtim);
  #else
    return fu_helper__to_timestamp(&tmp.st_mtime);
  #endif
}

FU_COMPILE_MODE fu_Timestamp fu_get_file_accessed_time(const fu_string FU_RESTRICT file_path){
  struct stat tmp;
  lstat(file_path, &tmp);
  #ifndef WIN32
    return fu_helper__to_timestamp(&tmp.st_atim);
  #else
    return fu_helper__to_timestamp(&tmp.st_atime);
  #endif
}

FU_COMPILE_MODE fu_index fu_get_file_size(const fu_string FU_RESTRICT file_path){
  struct stat tmp;
  lstat(file_path, &tmp);
  return tmp.st_size;
}


FU_COMPILE_MODE fu_string fu_helper__resolve_tabs(fu_string FU_RESTRICT str){
  const int TAB_SIZE = 4;
  fu_string og_str = str;

  fu_index tab_count = 0;
  for(fu_index i = 0; i < strlen(str); i++){
    tab_count += str[i] == '\t';
  }

  fu_index new_size = strlen(str) - tab_count + (tab_count * TAB_SIZE) + 1;
  fu_string new_buff = (fu_string)malloc(new_size);
  fu_string new_buff_og = new_buff;
  for(fu_index i = 0; i <= new_size; i++){
    if(str > og_str + strlen(og_str)){
      break;
    }
    if(*str == '\t'){
      for(unsigned char i1 = 0; i1 < TAB_SIZE; i1++){
        *new_buff = ' ';
        new_buff++;
      }
      str++;
    }else{
      *new_buff = *str;
      new_buff++;
      str++;
    }
  }
  new_buff_og[new_size - 1] = '\0';

  free(og_str);
  return new_buff_og;
}

FU_COMPILE_MODE fu_BinFile fu_load_bin_file(const fu_string FU_RESTRICT file_path){
  FILE *f = fopen(file_path, "r");
  #ifdef FU_DEBUG
    if(f == FU_NULL){
      fprintf(stderr, "ERR: fu_load_bin_file(): Couldn't load file \"%s\"\n", file_path);
      exit(-1);
    }
  #endif
  fseek(f, 0, SEEK_END);
  int fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  #ifdef FU_DEBUG
    if(fsize <= 0){
      fprintf(stderr, "ERR: fu_load_bin_file(%s): File size = 0!\n", file_path);
      fclose(f);
      exit(-1);
    }
  #endif

  fu_string ret_buffer = (fu_string)malloc(fsize);
  #ifdef FU_DEBUG
    if(ret_buffer == FU_NULL){
      fprintf(stderr, "ERR: fu_load_bin_file(%s): Out of memory! or Block too big\n", file_path);
      fclose(f);
      f = FU_NULL;
    }
  #endif
  //fsize = fread(ret_buffer, fsize, fsize, f);
  size_t fcharsread = fread(ret_buffer, 1, fsize, f);
  #ifdef FU_DEBUG
    if(fcharsread != fsize){
      fprintf(stderr, "ERR: fu_load_bin_file(%s): Bytes read is not equal to size of file! %i != %li\n", file_path, fsize, fcharsread);
    }
  #endif

  fclose(f);

  fu_BinFile ret;
  ret.bin = ret_buffer;
  ret.size = fsize;
  return ret;
}

FU_COMPILE_MODE fu_TextFile fu_load_text_file(const fu_string FU_RESTRICT file_path){
  fu_BinFile binfile = fu_load_bin_file(file_path);
  binfile.bin = (fu_string)realloc(binfile.bin, binfile.size + 1);
  fu_string buffer = binfile.bin;
  *(buffer + binfile.size) = '\0';
  binfile.size++;


  //Get total amount of lines
  fu_index nline_count = 1;
  for(fu_index i = 0; i < strlen(buffer); i++){
    if(buffer[i] == '\n'){
      nline_count++;
    }
  }
  //was there text at the end that we missed?
  if(buffer[strlen(buffer) - 1] != '\n')
    nline_count++;


  //create string array
  fu_string* ret_buff = (fu_string*)malloc(sizeof(fu_string) * nline_count);
  fu_index chars_processed = 0;
  for(fu_index i = 0; i + 1 < nline_count; i++){
    fu_index nline_strlen = 0;
    fu_index i1 = 0;
    while(1){
      if(chars_processed + 1 < binfile.size){
        if(buffer[i1] == '\n')
          break;
        nline_strlen++;
        i1++;
        chars_processed++;
        continue;
      }
      break;
    }

    ret_buff[i] = (fu_string)malloc(nline_strlen + 1);
    memcpy(ret_buff[i], buffer, nline_strlen);
    ret_buff[i][nline_strlen] = '\0';
    ret_buff[i] = fu_helper__resolve_tabs(ret_buff[i]);

    buffer += (nline_strlen + 1);
  }

  #ifndef FU_USE_STRUCT_FUNCTIONS
    fu_free_bin_file(binfile);
  #endif

  fu_TextFile ret;
  ret.text = ret_buff;
  ret.size = nline_count - 1;
  return ret;
}

FU_COMPILE_MODE fu_FilePerms fu_get_file_perms(const fu_string FU_RESTRICT file_path){
  fu_FilePerms ret;
  ret.chr_form = 0;
  errno = 0;

  int opcode = access(file_path, F_OK);
  ret.exists = (opcode == 0) && (errno != ENOENT);
  ret.not_accessible = (errno == EACCES);

  opcode = access(file_path, R_OK);
  ret.readable = (opcode == 0);

  opcode = access(file_path, W_OK);
  ret.writeable = (opcode == 0) && (errno != EACCES);
  ret.read_only = (opcode != 0) && (errno == EROFS);

  opcode = access(file_path, X_OK);
  ret.executable = (opcode == 0) && (errno != EACCES);

  return ret;
}

FU_COMPILE_MODE fu_FileType fu_get_file_type(const fu_string FU_RESTRICT file_path){
  struct stat fu_stat;
  lstat(file_path, &fu_stat);
  return fu_helper__get_file_type(&fu_stat);
}

FU_COMPILE_MODE void fu_save_bin_file(const fu_string FU_RESTRICT file_path, fu_BinFile bin){
  FILE* f = fopen(file_path, "wb");
  #ifdef FU_DEBUG
    if(!f){
      fprintf(stderr, "ERR: File could not open (fu_save_bin_file): %d \"%s\"\n", errno, file_path);
      exit(-1);
    }
  #endif

  fwrite(bin.bin, sizeof(char), bin.size, f);
  fclose(f);
}

FU_COMPILE_MODE void fu_save_text_file(const fu_string FU_RESTRICT file_path, fu_TextFile text){
  FILE* f = fopen(file_path, "wb");
  #ifdef FU_DEBUG
    if(!f){
      fprintf(stderr, "ERR: File could not open (fu_save_text_file): %d\n", errno);
      exit(-1);
    }
  #endif

  for(fu_index i = 0; i < text.size; i++){
    fwrite(text.text[i], sizeof(char), strlen(text.text[i]), f);
    fwrite("\n", sizeof(char), 1, f);
  }
  fclose(f);
}

//Allocate lines for a List
FU_COMPILE_MODE fu_List fu_alloc_list(fu_index element_count){
  fu_List ret;
  ret.size = element_count;
  ret.text = (fu_string*)malloc(sizeof(fu_string) * ret.size);
  return ret;
}

//Allocate lines for a text file
FU_COMPILE_MODE fu_TextFile fu_alloc_text_file(fu_index line_count){
  return (fu_TextFile)fu_alloc_list(line_count);
}

//Allocate lines for a bin file
FU_COMPILE_MODE fu_BinFile fu_alloc_bin_file(fu_index byte_count){
  fu_BinFile ret;
  ret.size = byte_count;
  ret.bin = (fu_string)malloc(byte_count);
  return ret;
}

FU_COMPILE_MODE fu_BinFile* fu_memcpy_bin_file(fu_BinFile * dest, fu_BinFile* FU_RESTRICT src){
  dest->size = src->size;
  dest->bin = (fu_string)malloc(src->size);
  dest->bin = (fu_string)memcpy(dest->bin, src->bin, src->size);
  return dest;
}

FU_COMPILE_MODE fu_TextFile* fu_memcpy_text_file(fu_TextFile * dest, fu_TextFile* FU_RESTRICT src){
  dest->size = src->size;

  dest->text = (fu_string*)malloc(sizeof(fu_string) * src->size);
  for(fu_index i = 0; i < src->size; i++){
    dest->text[i] = (fu_string)malloc(strlen(src->text[i]) + 1);
    strcpy(dest->text[i], src->text[i]);
  }
  return dest;
}

FU_COMPILE_MODE fu_List* fu_memcpy_list(fu_List * dest, fu_List* FU_RESTRICT src){
  return (fu_List*)(void*)fu_memcpy_text_file(dest, src);
}

FU_COMPILE_MODE fu_BinFile fu_create_bin_copy(fu_BinFile src){
  fu_BinFile ret;
  fu_memcpy_bin_file(&ret, &src);
  return ret;
}

FU_COMPILE_MODE fu_List fu_create_list_copy(fu_List src){
  fu_List ret;
  fu_memcpy_list(&ret, &src);
  return ret;
}

FU_COMPILE_MODE fu_TextFile fu_create_text_copy(fu_TextFile src){
  return (fu_TextFile)fu_create_list_copy(src);
}

/*
FU_COMPILE_MODE fu_List fu_join_list(fu_List * a, fu_List b){
  #ifdef FU_DEBUG
    if(b.size == 0){
      fprintf(stderr, "WARNING: fu_join_list(): Size of b was 0!\n");
      return *a;
    }
    if(b.text == FU_NULL){
      fprintf(stderr, "WARNING: fu_join_list(): b contains a null pointer!\n");
      return *a;
    }
    if(a == NULL){
      fprintf(stderr, "ERR: fu_join_list(): a == NULL!\n");
      abort();
    }
    if(a->text == NULL){
      fprintf(stderr, "ERR: fu_join_list(): a.text == NULL!\n");
      abort();
    }
    if(a->size == 0){
      fprintf(stderr, "ERR: fu_join_list(): 'a' was never allocated! a.size == 0!\n");
      abort();
    }
  #endif

  a->text = (fu_string*)realloc(a->text, sizeof(fu_string) * (a->size + b.size));

  fu_index i = a->size;
  fu_index beg_cursor = a->size;
  a->size += b.size;
  for(; i < a->size; i++){
    a->text[i] = FU_STRDUP(b.text[i - beg_cursor]);
    #ifdef FU_DEBUG
      if(a->text[i] == NULL){
        fprintf(stderr, "ERR: fu_join_list(): Out of RAM! i == %llu\n", i);
        abort();
      }
    #endif
  }

  return *a;
}*/

FU_COMPILE_MODE fu_List fu_join_list(fu_List * a, fu_List b){
  fu_List ret = *a;
  if(b.size == 0)
    return ret;
  if(b.text == FU_NULL)
    return ret;

  ret.text = (fu_string*)realloc(ret.text, sizeof(fu_string) * (ret.size + b.size));

  fu_index i = ret.size;
  fu_index beg_cursor = ret.size;
  ret.size += b.size;
  for(; i < ret.size; i++){
    ret.text[i] = FU_STRDUP(b.text[i - beg_cursor]);
  }

  *a = ret;
  return ret;
}

FU_COMPILE_MODE fu_List fu_join_list_copy(fu_List a, fu_List b){
  fu_List cpy = fu_create_list_copy(a);
  return fu_join_list(&cpy, b);
}

//Join 2 bin files together
FU_COMPILE_MODE fu_BinFile fu_join_bin(fu_BinFile* a, fu_BinFile b){
    #ifdef FU_DEBUG
    if(b.size <= 0){
      fprintf(stderr, "WARNING: fu_join_bin(): Size of b was 0!\n");
      return *a;
    }
    if(b.bin == FU_NULL){
      fprintf(stderr, "WARNING: fu_join_bin(): b contains a null pointer!\n");
      return *a;
    }
    if(a == FU_NULL){
      fprintf(stderr, "ERR: fu_join_bin(): a == NULL!\n");
      abort();
    }
    if(a->bin == FU_NULL){
      fprintf(stderr, "ERR: fu_join_bin(): a.bin == NULL!\n");
      abort();
    }
    if(a->size <= 0){
      fprintf(stderr, "ERR: fu_join_bin(): 'a' was never allocated! a.size == 0!\n");
      abort();
    }
  #endif
  #ifdef FU_DEBUG
    fu_index old_size = a->size;
  #endif
  a->size += b.size;
  
  #ifdef FU_DEBUG
    if(old_size >= a->size){
      fprintf(stderr, "ERR: fu_join_bin(): Oldsize of 'a' is bigger than new size: OLD = %llu, NEW = %llu, B_SIZE = %llu\n", old_size, a->size, b.size);
    }
  #endif

  a->bin = (fu_string)realloc(a->bin, a->size);
  fu_string cursor = a->bin + a->size - b.size;
  cursor = (fu_string)memcpy(cursor, b.bin, b.size);
  return *a;
}

FU_COMPILE_MODE fu_BinFile fu_join_bin_copy(fu_BinFile a, fu_BinFile b){
  fu_BinFile cpy = fu_create_bin_copy(a);
  return fu_join_bin(&cpy, b);
}


//Append text to the end of a text file
FU_COMPILE_MODE fu_TextFile fu_append_text(fu_TextFile * ptr, fu_string FU_RESTRICT str){
    return (fu_TextFile)fu_append_item(((fu_List*)ptr), str);
}
//Append text to the end of a list
FU_COMPILE_MODE fu_List fu_append_item(fu_List * ptr, fu_string FU_RESTRICT str){
    fu_List txt;
    txt.text = (fu_string*)malloc(sizeof(fu_string) * ptr->size);
    #ifdef FU_DEBUG
        if(txt.text == FU_NULL){
            fprintf(stderr, "WARNING: fu_append_text(ptr = %p, str = \"%s\"): Not enough RAM!! Aborting function ...\n", ptr, str);
            return *ptr;
        }
    #endif
    txt.text[0] = (fu_string)FU_STRDUP(str);
    #ifdef FU_DEBUG
        if(txt.text[0] == FU_NULL){
            fprintf(stderr, "WARNING: fu_append_text(ptr = %p, str = \"%s\"): Not enough RAM!! Aborting function ...\n", ptr, str);
            return *ptr;
        }
    #endif
    txt.size = 1;

    return (fu_List)fu_join_list(((fu_List*)ptr), txt);
}
//Insert text line at line 'i'
FU_COMPILE_MODE fu_TextFile fu_insert_text(fu_TextFile * ptr, fu_index i, fu_string FU_RESTRICT str){
    return fu_insert_item(((fu_List*)ptr), ((fu_index)i), ((fu_string)str));
}
//Insert text line at line 'i'
FU_COMPILE_MODE fu_List fu_insert_item(fu_List * ptr, fu_index i, fu_string FU_RESTRICT str){
    #ifdef FU_DEBUG
        if(str == FU_NULL){
            fprintf(stderr, "ERR: fu_insert_item(ptr = %p, i = %llu, str = %p): 'str' == NULL!!!\n", ptr, i, str);
            abort();
        }
        if(ptr == FU_NULL){
            fprintf(stderr, "ERR: fu_insert_item(ptr = %p, i = %llu, str = \"%s\"): ptr is NULL!!!\n", ptr, i, str);
            abort();
        }
        if(i >= ptr->size){
            fprintf(stderr, "ERR: fu_insert_item(ptr = %p, i = %llu, str = \"%s\"): 'i' is way to big!!!\n", ptr, i, str);
            abort();
        }
        if(ptr->size == 0){
            fprintf(stderr, "ERR: fu_insert_item(ptr = %p, i = %llu, str = \"%s\"): ptr list size is 0!!!\n", ptr, i, str);
            abort();
        }
        if((ptr->size + 1) == 0){
            fprintf(stderr, "ERR: fu_insert_item(ptr = %p, i = %llu, str = \"%s\"): ptr->list size rolled over to 0!!!\n", ptr, i, str);
            abort();
        }
    #endif

    fu_string* strarray = (fu_string*)malloc(sizeof(fu_string*) * (ptr->size + 1));
    #ifdef FU_DEBUG
        if(strarray == FU_NULL){
            fprintf(stderr, "WARNING: fu_insert_item(ptr = %p, i = %llu, str = \"%s\"): Not enough RAM!!! Function Aborting ...\n", ptr, i, str);
            return *ptr;
        }
    #endif
    strarray = (fu_string*)memcpy(strarray, ptr->text, sizeof(fu_string*) * ptr->size); //only copy pointers
    #ifdef FU_DEBUG
        if(strarray == FU_NULL){
            fprintf(stderr, "WARNING: fu_insert_item(ptr = %p, i = %llu, str = \"%s\"): Not enough RAM!!! Function Aborting ...\n", ptr, i, str);
            return *ptr;
        }
    #endif
    free(ptr->text);
    ptr->text = (fu_string*)strarray;

    for(fu_index index = ptr->size; index > i; index--){
        ptr->text[index] = (fu_string)ptr->text[index - 1];
    }
    ptr->text[i] = str;

    return *ptr;
}
//Deletes text line at line 'i'
FU_COMPILE_MODE fu_TextFile fu_delete_text(fu_TextFile * ptr, fu_index i){
    return (fu_TextFile)fu_delete_item(((fu_List*)ptr), i);
}
//Deletes text line at line 'i'
FU_COMPILE_MODE fu_List fu_delete_item(fu_List * ptr, fu_index i){
    #ifdef FU_DEBUG
        if(ptr == FU_NULL){
            fprintf(stderr, "ERR: fu_delete_item(ptr = %p, i = %llu): ptr is NULL!!!\n", ptr, i);
            abort();
        }
        if(i >= ptr->size){
            fprintf(stderr, "ERR: fu_delete_item(ptr = %p, i = %llu): 'i' is way to big!!!\n", ptr, i);
            abort();
        }
        if(ptr->size == 0){
            fprintf(stderr, "ERR: fu_delete_item(ptr = %p, i = %llu): ptr list size is 0!!!\n", ptr, i);
            abort();
        }
        if((ptr->size - 1) == 0){
            fprintf(stderr, "ERR: fu_delete_item(ptr = %p, i = %llu): ptr->list size rolled over to 0!!!\n", ptr, i);
            abort();
        }
    #endif

    fu_string* strarray = (fu_string*)malloc(sizeof(fu_string*) * (ptr->size - 1));
    #ifdef FU_DEBUG
        if(strarray == FU_NULL){
            fprintf(stderr, "WARNING: fu_delete_item(ptr = %p, i = %llu): Not enough RAM!!! Function Aborting ...\n", ptr, i);
            return *ptr;
        }
    #endif
    for(fu_index index = i; index < ptr->size; index++){
        ptr->text[index] = (fu_string)ptr->text[index + 1];
    }
    strarray = (fu_string*)memcpy(strarray, ptr->text, sizeof(fu_string*) * (ptr->size - 1)); //only copy pointers
    #ifdef FU_DEBUG
        if(strarray == FU_NULL){
            fprintf(stderr, "WARNING: fu_delete_item(ptr = %p, i = %llu): Not enough RAM!!! Function Aborting ...\n", ptr, i);
            return *ptr;
        }
    #endif
    free(ptr->text);
    ptr->text = (fu_string*)strarray;
    ptr->size--;

    return *ptr;
}

//Gets a new struct that contants elements 'b' to 'e'
FU_COMPILE_MODE fu_TextFile fu_sub_text(fu_TextFile file, fu_index b, fu_index e){
       return (fu_TextFile)fu_sub_list(*((fu_List*)&file), b, e);
}

//Gets a new struct that contants elements 'b' to 'e'
FU_COMPILE_MODE fu_List fu_sub_list(fu_List list, fu_index b, fu_index e){
       #ifdef FU_DEBUG
              if(list.text == FU_NULL){
                     fprintf(stderr, "ERR: fu_sub_list(file.text =  %p, b = %llu, e = %llu): List is not allocated!!\n", list.text, b, e);
                     abort();
              }
              if(list.size <= b){
                     fprintf(stderr, "ERR: fu_sub_list(file.text =  %p, b = %llu, e = %llu): 'b' is too big (file.size = %llu)\n", list.text, b, e, list.size);
                     abort();
              }
              if(list.size < e){
                     fprintf(stderr, "ERR: fu_sub_list(file.text =  %p, b = %llu, e = %llu): 'e' is too big (file.size = %llu)\n", list.text, b, e, list.size);
                     abort();
              }
              if(b >= e){
                     fprintf(stderr, "ERR: fu_sub_list(file.text =  %p, b = %llu, e = %llu): 'b' is bigger than 'e'!!!\n", list.text, b, e);
                     abort();
              }
       #endif
       
       fu_List ret;
       ret.size = e - b + 1;
       ret.text = (fu_string*)malloc(sizeof(fu_string*) * ret.size);
       #ifdef FU_DEBUG
              if(ret.text == FU_NULL){
                     fprintf(stderr, "ERR: fu_sub_list(file.text =  %p, b = %llu, e = %llu): Out of RAM!!!\n", list.text, b, e);
                     abort();
              }
       #endif
       
       
       for(fu_index i = b; i < b + ret.size; i++){
              #ifdef FU_DEBUG
                 if(list.text[i] == FU_NULL){
                            fprintf(stderr, "ERR: fu_sub_list(file.text =  %p, b = %llu, e = %llu): List element list.text[%llu] = NULL!!!\n", list.text, b, e, i);
                            abort();
                 }
              #endif
              ret.text[i - b] = (fu_string)malloc(sizeof(char) * strlen(list.text[i]) + 1);
              #ifdef FU_DEBUG
                     if(ret.text[i -  b] == FU_NULL){
                            fprintf(stderr, "ERR: fu_sub_list(file.text =  %p, b = %llu, e = %llu): Out of RAM!!!\n", list.text, b, e);
                            abort();
                     }
              #endif
              ret.text[i - b] = (fu_string)strcpy(ret.text[i - b], list.text[i]);
              #ifdef FU_DEBUG
                     if(ret.text[i - b] == FU_NULL){
                            fprintf(stderr, "ERR: fu_sub_list(file.text =  %p, b = %llu, e = %llu): Uknown strcpy error (i = %llu)!!!\n", list.text, b, e, i);
                            abort();
                     }
              #endif
       }
       return ret;
}

//Gets a new struct that contants elements 'b' to 'e'
FU_COMPILE_MODE fu_BinFile fu_sub_bin(fu_BinFile file, fu_index b, fu_index e){
       #ifdef FU_DEBUG
              if(file.bin == FU_NULL){
                     fprintf(stderr, "ERR: fu_sub_bin(file.bin=  %p, b = %llu, e = %llu): List is not allocated!!\n", file.bin, b, e);
                     abort();
              }
              if(file.size <= b){
                     fprintf(stderr, "ERR: fu_sub_bin(file.bin =  %p, b = %llu, e = %llu): 'b' is too big (file.size = %llu)\n", file.bin, b, e, file.size);
                     abort();
              }
              if(file.size <= e){
                     fprintf(stderr, "ERR: fu_sub_bin(file.bin =  %p, b = %llu, e = %llu): 'e' is too big (file.size = %llu)\n", file.bin, b, e, file.size);
                     abort();
              }
              if(b >= e){
                     fprintf(stderr, "ERR: fu_sub_bin(file.bin =  %p, b = %llu, e = %llu): 'b' is bigger than 'e'!!!\n", file.bin, b, e);
                     abort();
              }
       #endif
       
       fu_BinFile ret;
       ret.size = e - b;
       ret.bin = (fu_hexarray)malloc(sizeof(char) * ret.size);
       #ifdef FU_DEBUG
              if(ret.bin == FU_NULL){
                     fprintf(stderr, "ERR: fu_sub_bin(file.bin =  %p, b = %llu, e = %llu): Out of RAM!!!\n", file.bin, b, e);
                     abort();
              }
       #endif
       
       for(fu_index i = b; i < b + ret.size; i++){
              ret.bin[i - b] = file.bin[i];
       }
       return ret;
}


FU_COMPILE_MODE fu_index fu_get_hash(fu_BinFile file){
  fu_index ret = 0x12345;
  for(fu_index i = 0; i < file.size; i++){
    srandom(ret - i);
    ret = (ret + (random() % (file.bin[i] | 1))) << (random() % ((i | 1) % 100));
    ret |= ret + 0x20;
    ret -= file.bin[i];
  }
  return ret;
}

FU_COMPILE_MODE fu_BinFile fu_to_bin(fu_TextFile text_file){
  fu_index byte_buffer_size = 0;
  for(fu_index i = 0; i < text_file.size; i++){
    byte_buffer_size += strlen(text_file.text[i]) + (i + 1 < text_file.size);
  }

  fu_BinFile ret;
  ret.size = byte_buffer_size;
  ret.bin = (fu_string)malloc(byte_buffer_size);

  fu_string buffer = ret.bin;
  for(fu_index i = 0; i < text_file.size; i++){
    memcpy(buffer, text_file.text[i], strlen(text_file.text[i]));
    if(i + 1 < text_file.size){
      buffer[strlen(text_file.text[i])] = '\n';
    }
    buffer +=  strlen(text_file.text[i]) + (i + 1 < text_file.size);
  }
  return ret;
}

FU_COMPILE_MODE void fu_text_file_convert_unix(fu_TextFile txt_file){
  for(fu_index i = 0; i < txt_file.size; i++){
    fu_string tmp = txt_file.text[i];
    while (*tmp != '\0'){
      if(*tmp == '\r')
        *tmp = '\0';
      tmp++;
    }
  }
}

FU_COMPILE_MODE void fu_touch_file(const fu_string FU_RESTRICT file_path){
  #define CMD_STR "touch \""
  fu_string cmd = (fu_string)malloc(strlen(CMD_STR) + strlen(file_path) + 1);
  strcat(cmd, file_path);
  strcat(cmd, "\"");
  system(cmd);
  free(cmd);
}

FU_COMPILE_MODE fu_string fu_get_current_user(){
  return getenv("USER");
}

//Get list of all files with a given wildcard
FU_COMPILE_MODE fu_List fu_wildcard(fu_List contents, const fu_string FU_RESTRICT __format__){
  fu_List ret;
  ret.text = FU_NULL;
  ret.size = 0;

  //Dupe string
  fu_string format = FU_STRDUP(__format__);
  //fu_stringformat = (fu_string)malloc(strlen(__format__) + 1);
  //format = (fu_string)strcpy(format, __format__);

  fu_string before = format;
  fu_string after = FU_NULL;
  bool is_wildcard = false;
  for(fu_index i = 0; i < strlen(__format__); i++){
    if(format[i] == '*'){
      //Abuse how strings work to create 2 strings in one string
      format[i] = '\0';
      after = format + i + 1;
      is_wildcard = true;
      break;
    }
  }

  if(!is_wildcard){
    ret.text = FU_NULL;
    ret.size = 0;
    return ret;
  }


  fu_index wildcard_count = 0;

  fu_string cursor;
  fu_index before_strlen = strlen(before);
  fu_index after_strlen = strlen(after);
  for(fu_index i =0; i < contents.size; i++){
    if((cursor = strstr(contents.text[i], before)) != contents.text[i]) //begining string wasnt found
      continue;
    if((cursor = strstr(contents.text[i] + before_strlen, after)) == NULL) //ending string was not found
      continue;
    if(cursor + after_strlen < contents.text[i] + strlen(contents.text[i]))
      continue;
    wildcard_count++;
  }

  //Allocate string array
  if(contents.size > 0 && wildcard_count > 0){
    ret.text = (fu_string*)malloc(sizeof(fu_string) * wildcard_count);
    int wildcard_cusor = 0;
    for(fu_index i =0; i < contents.size; i++){
      if((cursor = strstr(contents.text[i], before)) != contents.text[i]) //begining string wasnt found
        continue;
      if((cursor = strstr(contents.text[i] + strlen(before), after)) == NULL) //ending string was not found
        continue;
      if(cursor + after_strlen < contents.text[i] + strlen(contents.text[i]))
        continue;
      ret.text[wildcard_cusor] = FU_STRDUP(contents.text[i]);
      wildcard_cusor++;
    }
    ret.size = wildcard_count;
  }

  free(format);
  return ret;
}

//Find files
FU_COMPILE_MODE fu_List fu_find(const fu_string FU_RESTRICT path, const fu_string FU_RESTRICT name, bool show_hidden, bool recursive){
  fu_List ret;

  fu_List empty;
  empty.size=0;
  empty.text = FU_NULL;

  fu_List dircontents = fu_get_dir_files((fu_string)path, false, show_hidden, false);
  ret = fu_wildcard(dircontents, name);

  fu_index element_count = 0;
  fu_index element_cursor = 0;
  if(ret.text != FU_NULL){
    //Add full path behind string
    for(fu_index i = 0; i < ret.size; i++){
      fu_string og_text = ret.text[i];
      ret.text[i] = (fu_string)malloc(strlen(path) + strlen(ret.text[i]) + 2);
      sprintf(ret.text[i], "%s/%s", path, og_text);
      //printf("%s\n", ret.text[i]);
      free(og_text);
    }
    fu_free_list(dircontents);
    goto fu_find__HELPER;
  }

  ret = empty;

  //Failed to find any wildcards

  for(fu_index i = 0; i < dircontents.size; i++){
    if(strstr(dircontents.text[i], name) == dircontents.text[i]){ //Found string
      element_count++;
    }
  }

  //Allocate array
  if(element_count > 0){
    ret.text = (fu_string*)malloc(sizeof(fu_string) * element_count);
  }

  for(fu_index i = 0; i < dircontents.size; i++){
    if(strstr(dircontents.text[i], name) == dircontents.text[i]){ //Found string
      ret.text[element_cursor] = (fu_string)malloc(strlen(dircontents.text[i]) + strlen(path) + 1 + 1);
      ret.text[element_cursor] = (fu_string)strcpy(ret.text[element_cursor], path);
      ret.text[element_cursor] = (fu_string)strcat(ret.text[element_cursor], "/");
      ret.text[element_cursor] = (fu_string)strcat(ret.text[element_cursor], dircontents.text[i]);
      element_cursor++;
    }
  }
  fu_free_list(dircontents);

  if(element_cursor <= 0){
    ret.text = FU_NULL;
  }

  fu_find__HELPER:
  if(recursive){
    fu_List folders = fu_get_dir_folders((fu_string)path, true, show_hidden, false);

    for(fu_index i = 0; i < folders.size; i++){
      fu_string buffer = FU_STRDUP(folders.text[i]);
      struct stat sbuff;
      lstat(buffer, &sbuff);
	 #ifndef WIN32
      if(S_ISLNK(sbuff.st_mode)){ //Do not follow links
        free(buffer);
        continue;
      }
	 #endif
      if(fu_get_file_perms((const fu_string)buffer).not_accessible){ //Do we not have permissions?
        free(buffer);
        continue;
      }

      fu_List files = fu_find(buffer, name, show_hidden, recursive); //Get file names (no full path)

      if(ret.text != FU_NULL && ret.size != 0){
        fu_join_list(&ret, files);
        fu_free_list(files);
      }else{
        fu_free_list(ret);
        ret = files;
      }

      free(buffer);
    }

    fu_free_list(folders);
  }

  return ret;
}

#ifdef FU_USE_STRUCT_FUNCTIONS //CPP features


//fu_BinFile
fu_BinFile::fu_BinFile(){
  this->size = 0;
  this->bin = nullptr;
}

fu_BinFile::fu_BinFile(fu_index bsize, fu_stringbuffer){
  this->size = 0;
  this->bin = nullptr;
}

fu_BinFile::fu_BinFile(const fu_stringpath){
  *this = fu_load_bin_file(path);
}

fu_index fu_BinFile::get_hash(){
  return fu_get_hash(*this);
}

char fu_BinFile::operator[](fu_index index){
  return this->bin[index];
}

fu_BinFile::~fu_BinFile(){
  if(this->bin == nullptr){
    free(this->bin);
    this->bin = nullptr;
  }
}


//fu_List
fu_List::fu_List(){
  this->size = 0;
  this->text = nullptr;
}

fu_List::fu_List(const fu_stringpath){
  this->load_text_file(path);
}

fu_List::fu_List(fu_index lines, fu_string* items){
  this->size = lines;
  this->text = items;
}

void fu_List::load_text_file(const fu_string path){
  if(this->text != nullptr){
    fu_free_list(*this);
  }
  fu_List tmp = fu_load_text_file(path);
  *this = tmp;
  tmp.text = nullptr;
}

fu_BinFile fu_List::to_bin(){
  fu_List& reference = *this;
  return fu_to_bin(reference);
}

fu_stringfu_List::operator[](fu_index index){
  return this->text[index];
}

fu_List::~fu_List(){
  if(this->text != nullptr){
    fu_List& reference = *this;
    fu_free_list(reference);
    reference.text = nullptr;
  }
}


//FileStats
fu_FileStats::~fu_FileStats(){
  if(file_name != nullptr)
    free(this->file_name);
}
#endif

#endif

#endif
