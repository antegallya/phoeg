#include <iostream>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"

#ifndef INV
#error You need to define the INV macro to select your invariant : -DINV=invFunction
#endif

int main(int argc, const char* argv[]) {
    for (std::string sig; std::getline(std::cin, sig);) {
        phoeg::Graph g = phoeg::convertFromGraph6(sig);
        long val = phoeg::INV(g);
        if (val == INF)
            continue;
        std::cout << sig << "\t" << val << std::endl;
    }
}
