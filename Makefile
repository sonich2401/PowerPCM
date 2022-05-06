CC = gcc
CCX = g++




ifndef UNAME
UNAME = $(shell uname)

MA_AUDIO_ARGS = -ldl -lm
PIXEL_GAME_ENGINE_ARGS = -lX11 -lGL -lpthread -lstdc++fs -std=c++17
CC_ARGS =  -Wno-unused-result -Wno-unknown-pragmas -Wno-incompatible-pointer-types $(MA_AUDIO_ARGS)
CCX_ARGS = -Wno-unused-result -Wno-unknown-pragmas $(PIXEL_GAME_ENGINE_ARGS) -ldl -lm

DEFINES_DEF = DATE="\"`date`\"" VER="\"$(VERSION)\"" PROG_NAME="\"$(BUILD)\"" OS_CLEAR="\"$(OS_CLEAR)\"" FORCE_EXPERIMENTAL_FS
else
PIXEL_GAME_ENGINE_ARGS =  -lpthread -luser32 -lgdi32 -lopengl32 -lgdiplus  -ldwmapi -lstdc++fs -std=c++17
CC_ARGS = -std=c++17  -static $(PIXEL_GAME_ENGINE_ARGS)
DEFINES_DEF = DATE="\"`date`\"" VER="\"$(VERSION)\"" PROG_NAME="\"$(BUILD)\"" OS_CLEAR="\"$(OS_CLEAR)\"" 
endif

ifneq ($(UNAME), Linux)
#CC = wine "/mnt/Windows/Program Files (x86)/CodeBlocks/MinGW/bin/g++.exe"
CC = x86_64-w64-mingw32-g++-posix
endif

ODIR := build
ROOT_DIRECTORY=.
SOURCES := ${shell find ${ROOT_DIRECTORY} -type d -print}
INCLUDE := ./include

C_FILES := $(foreach dir,$(SOURCES),  $(wildcard $(dir)/*.c) )
C_FILES := $(filter-out ./include/sound/stb_vorbis.c,$(C_FILES))
CPP_FILES := $(foreach dir,$(SOURCES),  $(wildcard $(dir)/*.cpp) ) #$(wildcard *.cpp)
H_FILES := $(foreach dir,$(SOURCES),  $(wildcard $(dir)/*.h) ) $(wildcard *.h)

ASM_FILES := $(foreach dir,$(SOURCES),  $(wildcard $(dir)/*.asm) ) $(wildcard *.asm)
S_FILES := $(foreach dir,$(SOURCES),  $(wildcard $(dir)/*.s) ) $(wildcard *.s)

O_FILES = $(abspath $(addprefix $(ODIR)/, $(CPP_FILES:.cpp=.o))) $(abspath $(addprefix $(ODIR)/, $(C_FILES:.c=.o)))



VERSION := 0.0
ifeq ($(UNAME), Linux)
	BUILD := $(shell basename $(CURDIR)).linux
	DEFINE_PREFIX = -
	OS_CLEAR = clear
else
	BUILD := $(shell basename $(CURDIR)).exe
	DEFINE_PREFIX = -
	OS_CLEAR = cls
endif

FORCE_EXPERIMENTAL_FS=true

ifndef $(DEBUG)
DEBUG = false
endif

ifeq ($(DEBUG), true)
DEFINES_DEF += DEBUG=$(DEBUG)
CC_ARGS += -g
CCX_ARGS += -g

CC_ARGS += -Wall
CCX_ARGS += -Wall
else
CC_ARGS += -Ofast
#CCX_ARGS += -Ofast
endif

DEFINES = $(foreach def,$(DEFINES_DEF), $(DEFINE_PREFIX)D$(def))

ifeq ($(UNAME), Windows)
DEFINES = 
endif
$(ODIR)/./%.o : %.cpp
	@echo $(notdir $(basename $*)).cpp
	@mkdir -p $(dir $@)
	@$(CCX) -c $*.cpp -o $@ -I $(INCLUDE) $(CCX_ARGS) $(DEFINES)

$(ODIR)/./%.o : %.c
	@echo $(notdir $(basename $*)).c
	@mkdir -p $(dir $@)
	@$(CC) -c $*.c -o $@ -I $(INCLUDE) $(CC_ARGS) $(DEFINES)

.PHONY: all
.PHONY: message
.PHONY: compile
.PHONY: wine_release
.PHONY: wine_deubg
.PHONY: build
.PHONY: debug
.PHONY: release
.PHONY: ok

$(BUILD): debug

all: clean compile run


compile: message $(addprefix $(ODIR)/, $(CPP_FILES:.cpp=.o)) $(addprefix $(ODIR)/, $(C_FILES:.c=.o)) | $(ODIR)
	@$(CCX) $(O_FILES) -o $(BUILD) -I $(INCLUDE) $(CC_ARGS) $(CCX_ARGS) $(DEFINES)
#@strip $(BUILD) -o $(BUILD).tmp
#@mv $(BUILD).tmp $(BUILD)

message:
	@echo Building ...
	@echo "\tDebug Build = $(DEBUG)"
	@echo "\tTarget = $(UNAME)"
	@echo "\tVersion = $(VERSION)"
	@echo "\tC++ Files to Compile = $(words $(CPP_FILES))"
	@echo "\tC Files to Compile = $(words $(C_FILES))"
	
$(ODIR):
	@mkdir -p $@

link:
	@echo $(CCX) $(O_FILES) -o $(BUILD) -I $(INCLUDE) $(CC_ARGS) $(CCX_ARGS) $(DEFINES)

run:
	@./$(BUILD)

clean:
	@rm -rf $(ODIR)
	@rm -rf x64
	@rm -rf Debug
	@rm -rf Release
	@rm -rf .vs
	@rm -rf $(BUILD)

debug:
	@$(MAKE) compile DEBUG=true --no-print-directory

release:
	@$(MAKE) compile --no-print-directory


compile1:
	@$(foreach cc,$(CPP_FILES),  $(shell $(CC) -I $(INCLUDE) $(CC_ARGS) $(DEFINES) -c $(cc) -o $(ODIR)/$(notdir $(basename $(cc)).o)))
	@$(CC) -o $(BUILD) -I $(INCLUDE) $(CC_ARGS) $(DEFINES) $(O_FILES)
compile2:
#@$(CC) -o $(BUILD) -I $(INCLUDE) $(CC_ARGS) $(DEFINES) $(CPP_FILES)
	@$(CC) $(CPP_FILES) -o $(BUILD) -I $(INCLUDE) $(CC_ARGS) $(DEFINES) 

#rm $(find ./$(ODIR)/ -name *.o)




wine_debug:
	@$(MAKE) UNAME=Windows DEBUG=true --no-print-directory
wine_release:
	@$(MAKE) UNAME=Windows --no-print-directory


#FOR DEBUGGGING PURPOSES
print:
#@echo $(CC) -o $(BUILD) -I $(INCLUDE) $(CC_ARGS) $(DEFINES) $(CPP_FILES)
	@echo $(CC) $*.cpp -o $@ -I $(INCLUDE) $(CC_ARGS) $(DEFINES)
	
print_H:
	@echo $(H_FILES)
print_O:
	@echo $(O_FILES)
print_C:
	@echo $(C_FILES)
print_CPP:
	@echo $(CPP_FILES)
print_dir:
	@echo $(dir include/resource/resources.h)
print_CC:
	@echo $(UNAME)
