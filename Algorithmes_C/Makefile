GCC_FLAGS=-Wall -g 
EXEC= main
LIB_FOLDER = Librairies

all: $(EXEC)

Array.o : Array.c
	gcc ${GCC_FLAGS} -c Array.c

GraphGenerator.o : GraphGenerator.c
	gcc ${GCC_FLAGS} -c GraphGenerator.c

Tools.o : Tools.c
	gcc ${GCC_FLAGS} -c Tools.c

Python.o : Python.c
	gcc ${GCC_FLAGS} -c Python.c

main.o : main.c 
	gcc ${GCC_FLAGS} -c main.c

main : main.o Array.o GraphGenerator.o Tools.o Python.o
	gcc ${GCC_FLAGS} -o $(EXEC) main.o Array.o GraphGenerator.o Tools.o Python.o -lm

lib : Array.c GraphGenerator.c Tools.c Python.c
	gcc -Wall -fPIC -c GraphGenerator.c
	gcc -Wall -fPIC -c Array.c
	gcc -Wall -fPIC -c Tools.c
	gcc -Wall -fPIC -c Python.c
	gcc -shared -o $(LIB_FOLDER)/lib.so GraphGenerator.o Array.o Tools.o Python.o

open : 
	geany *c *h

clean:
	rm -f *.o $(EXEC) *.exe $(LIB_FOLDER)/*so
