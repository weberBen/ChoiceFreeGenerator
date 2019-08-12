GCC_FLAGS=-Wall -g 

FOLDER_GRAPH = Graph
EXEC= main
FOLDER_PY = Py_interface
EXEC_PY = main_py

OBJDIR=obj
vpath %.c src
vpath %.h src

all: $(EXEC) $(EXEC_PY)


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

main_py : Array.o GraphGenerator.o Tools.o Rand.o Python.o Server.o PythonMain.o Display.o
	gcc ${GCC_FLAGS} -o $(EXEC_PY) PythonMain.o Python.o Server.o  Array.o GraphGenerator.o Tools.o Rand.o Display.o -lm -lglpk


#Tools option
open : 
	geany $(FOLDER_GRAPH)/*c $(FOLDER_GRAPH)/*h

clean:
	rm -f *.o
