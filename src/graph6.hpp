#ifndef GRAPH6_HPP
#define GRAPH6_HPP

#include <boost/graph/adjacency_matrix.hpp>

namespace phoeg
{

    typedef boost::adjacency_matrix<boost::undirectedS> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor vertex;
    typedef boost::graph_traits<Graph>::edge_descriptor edge;
    typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
    typedef boost::graph_traits<Graph>::edge_iterator edge_iter;

    std::string convertToGraph6(const Graph & g);

    Graph convertFromGraph6(const std::string & s);

}

#endif
