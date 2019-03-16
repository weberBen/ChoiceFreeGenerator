GCC_FLAGS=-Wall -g 
EXEC= main
EXEC_Py = py_main
LIB_FOLDER = Librairies
PY_LIB = py_lib.so
LOCATION = /home/benjamin/Projets/FreeChoiceGenerator/Algorithmes_C

all: $(EXEC)

Array.o : Array.c
	gcc ${GCC_FLAGS} -c Array.c

GraphGenerator.o : GraphGenerator.c
	gcc ${GCC_FLAGS} -c GraphGenerator.c

Tools.o : Tools.c
	gcc ${GCC_FLAGS} -c Tools.c

Python.o : Python.c
	gcc ${GCC_FLAGS} -c Python.c

Server.o : Server.c
	gcc ${GCC_FLAGS} -c Server.c

main.o : main.c 
	gcc ${GCC_FLAGS} -c main.c

main : main.o Array.o GraphGenerator.o Tools.o Python.o Server.o py_main
	gcc ${GCC_FLAGS} -o $(EXEC) main.o Array.o GraphGenerator.o Tools.o Python.o Server.o -lm

PythonMain.o : PythonMain.c 
	gcc ${GCC_FLAGS} -c PythonMain.c

py_main : PythonMain.o Array.o GraphGenerator.o Tools.o Python.o Server.o
	gcc ${GCC_FLAGS} -o $(EXEC_Py) PythonMain.o Array.o GraphGenerator.o Tools.o Python.o Server.o -lm

open : 
	geany *c *h

clean:
	rm -f *.o $(EXEC) $(EXEC_Py) *.exe
