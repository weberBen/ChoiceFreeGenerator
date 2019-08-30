GCC_FLAGS=-Wall -g -Werror -std=c99 -pedantic-errors

path_to_source_folder = ../../../
LIB_FOLDER = Library
LIB_NAME = choiceFreeGeneratorDevLib.a
LIB_PATH = $(LIB_FOLDER)/$(LIB_NAME)
current_dir = $(path_to_source_folder)/Algorithmes_C/


all: main

main : clean
	gcc ${GCC_FLAGS} -o main main.c $(current_dir)$(LIB_PATH) -I$(current_dir)$(LIB_FOLDER) -lm -lglpk


clean:
	rm -f *.o main
