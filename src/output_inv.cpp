#include <iostream>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"
#include "nauty_interface.hpp"

int main(int argc, const char* argv[]) {
    std::string sig;
    long v;
    for (sig; std::getline(std::cin, sig);)
    {
        phoeg::Graph g = phoeg::convertFromGraph6(sig);
        if (phoeg::isConnected(g))
        {
            v = phoeg::eccentricConnectivity(g);
            std::cout << sig << "\t" << v << "\n";
        }
  }
}
