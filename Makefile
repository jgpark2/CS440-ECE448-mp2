#
EXE = mp2
OBJS = main.o

COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall -Werror
LINKER = g++
#LINKER_OPTS = -lpng

all : $(EXE)

$(EXE) : $(OBJS)
	$(LINKER) $(OBJS) -o $(EXE)

student.o : student.h student.cpp
	$(COMPILER) $(COMPILER_OPTS) student.cpp
	
clean :
	-rm -f *.o $(EXE)
