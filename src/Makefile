CC=g++
CFLAGS=-pedantic -Wall -c -W
CPPFLAGS=-W -std=c++11
LDFLAGS=-lboost_system -lboost_timer
OBJ= Solver.o main.o
OBJTEST= unitTest.o maintest.o
INSTALL=/usr/bin/install -c
BINDIR=/usr/local/bin
EXEC=project
EXECTEST=test

HEADERS=coloringProblem.hpp graphFeatures.hpp invariants.hpp graph6.hpp \
	graph.hpp SAT.h Minisat/Solver.hpp

G++ = g++
CLEANPROJECT=cleanProject
CLEAN = clean
all: main

#
# Dev Tools
#

.PHONY: solution

# Clean project

cleanProject:
	$(RM) *.o main
	
clean: $(CLEANPROJECT)

#
# Compile
#

main: $(OBJ)
	$(G++) $(LDFLAGS) *.o -o main 

Solver.o: Minisat/Solver.cpp Minisat/Solver.hpp Minisat/SolverTypes.hpp Minisat/Global.hpp Minisat/Heap.hpp
	$(G++) $(CPPFLAGS) -c Minisat/Solver.cpp 

main.o: main.cpp $(HEADERS)
	$(G++) $(CPPFLAGS) -c main.cpp

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<  
	








	

