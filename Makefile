cpp = g++
opt = --std=c++11
dst = build
src = src

libs = graph.hpp invariants.hpp transformations.hpp

exec = test.cpp

test = testsBoost.cpp

default : prepare test

prepare :
	test -d $(dst) || mkdir $(dst)

build :
	$(cpp) $(opt) -o $(dst)/$(basename $(exec)) $(src)/$(exec) $(addprefix $(src)/, $(libs))

test :
	$(cpp) $(opt) -o $(dst)/$(basename $(test)) $(src)/$(test) $(addprefix $(src)/, $(libs))

clean:
	rm -rf $(dst)

