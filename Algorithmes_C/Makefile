GCC_FLAGS=-Wall -g -Werror -std=c99 -pedantic-errors
EXTERNAL_LIB=-lm -lglpk

FOLDER_GRAPH = Graph
EXEC= choiceFreeGenerator
FOLDER_PY = Py_interface
EXEC_PY = main_py
LIB_FOLDER = Library
EXAMPLES_FOLDER = Examples
LIB_NAME = choiceFreeGeneratorLib.a
DEV_LIB_NAME = choiceFreeGeneratorDevLib.a
LIB_PATH = $(LIB_FOLDER)/$(LIB_NAME)
LIB_DEV_PATH = $(LIB_FOLDER)/$(DEV_LIB_NAME)
current_dir = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJDIR=obj
vpath %.c src
vpath %.h src

all: main $(EXEC_PY) lib examples


#Lib for generating graph
Array.o : $(FOLDER_GRAPH)/Array.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/Array.c $(EXTERNAL_LIB)

GraphGenerator.o : $(FOLDER_GRAPH)/GraphGenerator.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/GraphGenerator.c $(EXTERNAL_LIB)

Display.o : $(FOLDER_GRAPH)/Display.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/Display.c $(EXTERNAL_LIB)

Tools.o : $(FOLDER_GRAPH)/Tools.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/Tools.c $(EXTERNAL_LIB)

Rand.o : $(FOLDER_GRAPH)/Rand.c
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/Rand.c $(EXTERNAL_LIB)

#Lib to communicate with pyhton (for graphical display)
Server.o : $(FOLDER_PY)/Server.c
	gcc ${GCC_FLAGS} -c $(FOLDER_PY)/Server.c $(EXTERNAL_LIB)

Python.o : $(FOLDER_PY)/Python.c
	gcc ${GCC_FLAGS} -I$(FOLDER_GRAPH) -c $(FOLDER_PY)/Python.c $(FOLDER_GRAPH)/*.c $(EXTERNAL_LIB)
	#-I -> specify header path (for include)

#Static library
lib : Array.o Display.o GraphGenerator.o Rand.o Tools.o
	ar -crs $(LIB_PATH) Array.o Display.o GraphGenerator.o Rand.o Tools.o
	ar -crs $(LIB_DEV_PATH) Array.o Display.o GraphGenerator.o Rand.o Tools.o

#Examples
examples :
	gcc ${GCC_FLAGS} -o example1 $(EXAMPLES_FOLDER)/example.c $(LIB_PATH) -I$(current_dir)$(LIB_FOLDER) $(EXTERNAL_LIB)

#executables
	#Graph
main.o : $(FOLDER_GRAPH)/main.c 
	gcc ${GCC_FLAGS} -c $(FOLDER_GRAPH)/main.c $(EXTERNAL_LIB)

main : main.o Array.o GraphGenerator.o Tools.o Rand.o Display.o
	gcc ${GCC_FLAGS} -o $(EXEC) main.o Array.o GraphGenerator.o Tools.o Rand.o Display.o $(EXTERNAL_LIB)

	#Python
PythonMain.o : $(FOLDER_PY)/PythonMain.c 
	gcc ${GCC_FLAGS} -I$(FOLDER_GRAPH) -c $(FOLDER_PY)/PythonMain.c $(EXTERNAL_LIB)
	#-I -> specify header path (for include)

$(EXEC_PY) : Array.o GraphGenerator.o Tools.o Rand.o Python.o Server.o PythonMain.o Display.o
	gcc ${GCC_FLAGS} -o $(EXEC_PY) PythonMain.o Python.o Server.o  Array.o GraphGenerator.o Tools.o Rand.o Display.o $(EXTERNAL_LIB)


clean:
	rm -f *.o $(EXEC) $(EXEC_PY) $(LIB_PATH) $(LIB_DEV_PATH) example1
