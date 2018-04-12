#include <iostream>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"
#include "nauty_interface.hpp"

int main(int argc, const char* argv[]) {
    const int bufcap = 1000000;
    std::vector<std::string> sigs;
    std::vector<long> values;
    sigs.reserve(bufcap);
    values.reserve(bufcap);
    std::string sig;
    bool cont = true;
    while (cont) {
        /* Fill the buffer */
        int i = 0;
        sigs.clear();
        values.clear();
        for (; i < bufcap && cont; ++i) {
            cont = std::getline(std::cin, sig).good();
            sigs.push_back(sig);
            values.push_back(INF);
        }
        /* Buffer is full. Process it */
        #pragma omp parallel for shared(sigs, values)
        for (int j = 0; j < sigs.size(); ++j) {
            phoeg::Graph g = phoeg::convertFromGraph6(sigs[j]);
            values[j] = phoeg::semiTotalDominationNumber(g);
        }
        std::cerr << "Output " << sigs.size() << " values" << std::endl;
        for (i = 0; i < sigs.size(); ++i) {
            if (values[i] != INF)
                std::cout << sigs[i] << "\t" << values[i] << "\n";
        }
        std::cerr << "Done" << std::endl;
    }
}
