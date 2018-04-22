#include <iostream>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"

int main(int argc, const char* argv[]) {
    for (std::string sig; std::getline(std::cin, sig);) {
	phoeg::Graph g = phoeg::convertFromGraph6(sig);
	long val = phoeg::radius(g);
	if (val == INF)
	    continue;
        std::cout << sig << "\t" << val << std::endl;
    }
}
