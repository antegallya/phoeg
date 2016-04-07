#ifndef PHOEG_GRAPH_H
#define PHOEG_GRAPH_H

#include <iostream>
#include <boost/functional/hash.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_matrix.hpp>

//using namespace boost;

namespace phoeg
{

  typedef boost::adjacency_matrix<boost::undirectedS> Graph;

  typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex;

  typedef typename boost::graph_traits<Graph>::vertices_size_type vertices_size_t;

  typedef typename boost::graph_traits<Graph>::vertex_iterator vertex_iter;

}

#endif
