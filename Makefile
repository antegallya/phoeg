cpp = g++
opt = --std=c++11
dst = build
src = src

libs = graph.hpp invariants.hpp transformations.hpp nauty.h nauty.a nautyInterface.hpp graph6.hpp

exec = test.cpp

test = testsBoost.cpp

nauty = testNauty.cpp

default : prepare nauty

prepare :
	test -d $(dst) || mkdir $(dst)

build :
	$(cpp) $(opt) -o $(dst)/$(basename $(exec)) $(src)/$(exec) $(addprefix $(src)/, $(libs))

test :
	$(cpp) $(opt) -o $(dst)/$(basename $(test)) $(src)/$(test) $(addprefix $(src)/, $(libs))

nauty :
	$(cpp) $(opt) -o $(dst)/$(basename $(nauty)) $(src)/$(nauty) $(addprefix $(src)/, $(libs))

clean:
	rm -rf $(dst)

