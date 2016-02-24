#ifndef GRAPH6_HPP
#define GRAPH6_HPP

#include "graph.hpp"

namespace phoeg
{

    std::string convertToGraph6(const Graph & g);

    Graph convertFromGraph6(const std::string & s);

}

#endif
