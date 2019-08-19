GCC_FLAGS=-Wall -g -Werror -std=c99 -pedantic-errors

FOLDER_GRAPH = Graph
EXEC= freeChoiceGenerator
FOLDER_PY = Py_interface
EXEC_PY = main_py
LIB_FOLDER = Library
EXAMPLES_FOLDER = Examples
LIB_NAME = freeChoiceGeneratorLib.a
LIB_PATH = $(LIB_FOLDER)/$(LIB_NAME)
current_dir = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJDIR=obj
vpath %.c src
vpath %.h src

all: main $(EXEC_PY) lib examples


#Lib for generating graph
Array.o : $(FOLDER_GRAPH)/Array.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/Array.c

GraphGenerator.o : $(FOLDER_GRAPH)/GraphGenerator.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/GraphGenerator.c

Display.o : $(FOLDER_GRAPH)/Display.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/Display.c

Tools.o : $(FOLDER_GRAPH)/Tools.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/Tools.c

Rand.o : $(FOLDER_GRAPH)/Rand.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/Rand.c

#Lib to communicate with pyhton (for graphical display)
Server.o : $(FOLDER_PY)/Server.c
	gcc ${GCC_FLAGS} -c $(FOLDER_PY)/Server.c

Python.o : $(FOLDER_PY)/Python.c
	gcc ${GCC_FLAGS} -I$(FOLDER_GRAPH) -c $(FOLDER_PY)/Python.c $(FOLDER_GRAPH)/*.c
	#-I -> specify header path (for include)

#Static library
lib : Array.o Display.o GraphGenerator.o Rand.o Tools.o
	ar -crs $(LIB_PATH) Array.o Display.o GraphGenerator.o Rand.o Tools.o

#Examples
examples : lib
	gcc ${GCC_FLAGS} -o example1 $(EXAMPLES_FOLDER)/example.c -lm -lglpk $(LIB_PATH) -I$(current_dir)$(LIB_FOLDER)

#executables
	#Graph
main.o : $(FOLDER_GRAPH)/main.c 
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/main.c 

main : main.o Array.o GraphGenerator.o Tools.o Rand.o Display.o
	gcc ${GCC_FLAGS} -o $(EXEC) main.o Array.o GraphGenerator.o Tools.o Rand.o Display.o -lm -lglpk

	#Python
PythonMain.o : $(FOLDER_PY)/PythonMain.c 
	gcc ${GCC_FLAGS} -I$(FOLDER_GRAPH) -c $(FOLDER_PY)/PythonMain.c
	#-I -> specify header path (for include)

$(EXEC_PY) : Array.o GraphGenerator.o Tools.o Rand.o Python.o Server.o PythonMain.o Display.o
	gcc ${GCC_FLAGS} -o $(EXEC_PY) PythonMain.o Python.o Server.o  Array.o GraphGenerator.o Tools.o Rand.o Display.o -lm -lglpk


#Tools option
open : 
	geany $(FOLDER_GRAPH)/*c $(FOLDER_GRAPH)/*h

clean:
	rm -f *.o $(EXEC) $(EXEC_PY) $(LIB_PATH) example1
