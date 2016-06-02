CC=g++
CFLAGS=-pedantic -Wall -c -W
LDFLAGS=-lcppunit
OBJ= Solver.o Main.o
OBJTEST= unitTest.o maintest.o
INSTALL=/usr/bin/install -c
BINDIR=/usr/local/bin
EXEC=project
EXECTEST=test

G++ = g++ 
SOL=solution
CLEANPROJECT=cleanProject
CLEAN = clean
all: $(CLEAN) $(OBJ) $(SOL)

#
# Dev Tools
#

# Clean project

cleanProject:
	$(RM) *.o main
	
clean: $(CLEANPROJECT)

#
# Compile
#

solution: main.o Solver.o
	$(G++) *.o -o main 

Solver.o: Minisat/Solver.cpp Minisat/Solver.hpp Minisat/SolverTypes.hpp Minisat/Global.hpp Minisat/Heap.hpp
	$(G++) -c Minisat/Solver.cpp 

Main.o: main.cpp Minisat/Solver.hpp 
	$(G++) -c main.cpp

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<  
	
  
    








	

