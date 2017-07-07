#include <iostream>
#include <boost/graph/graph_utility.hpp>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"

void describe(phoeg::Graph& g, const char* name)
{
    std::cout << "vertex set: ";
    boost::print_vertices(g, name);
    std::cout << "\n";

    std::cout << "edge set: ";
    print_edges(g, name);
    std::cout << "\n";

    std::cout << "incident edges: " << "\n";
    print_graph(g, name);
    std::cout << "\n";

    std::cout << "NumCol: " << phoeg::numCol(g) << "\n";
}

int main(int, char*[])
{
    const char* name = "abcdefghij";

    phoeg::Graph g = phoeg::convertFromGraph6("IsP@OkWHG");

    describe(g, name);

    int k = 3;
    bool krho = phoeg::detail::is_k_colorable(g, k, true);
    std::cout << "Is "
              << k << " robust-colorable: " << krho << "\n";

    std::cout << "Its numcol-" << k << " is: " << phoeg::numColK(g, k) << "\n";

    return 0;
}
