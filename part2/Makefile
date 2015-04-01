#
EXE = mp2
OBJS = move.o gameTree.o board.o main.o

COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall -Werror
LINKER = g++
#LINKER_OPTS = -lpng

all : $(EXE)

$(EXE) : $(OBJS)
	$(LINKER) $(OBJS) -o $(EXE)

move.o : move.h move.cpp
	$(COMPILER) $(COMPILER_OPTS) move.cpp

gameTree.o : gameTree.h gameTree.cpp
	$(COMPILER) $(COMPILER_OPTS) gameTree.cpp

board.o : board.h board.cpp
	$(COMPILER) $(COMPILER_OPTS) board.cpp

main.o : main.cpp board.h
	$(COMPILER) -c $(COMPILER_OPTS) main.cpp
	
clean :
	-rm -f *.o $(EXE)
