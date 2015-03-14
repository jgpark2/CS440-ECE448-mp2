#
EXE = mp2
OBJS = student.o course.o main.o

COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall -Werror
LINKER = g++
#LINKER_OPTS = -lpng

all : $(EXE)

$(EXE) : $(OBJS)
	$(LINKER) $(OBJS) -o $(EXE)

student.o : student.h student.cpp
	$(COMPILER) $(COMPILER_OPTS) student.cpp

course.o : course.h course.cpp
	$(COMPILER) $(COMPILER_OPTS) course.cpp

main.o : main.cpp student.h
	$(COMPILER) -c $(COMPILER_OPTS) main.cpp
	
clean :
	-rm -f *.o $(EXE)