#include <iostream>
#include <boost/graph/graph_utility.hpp>
#include "graph.hpp"
#include "transformations.hpp"
#include "invariants.hpp"

using namespace std;
using namespace boost;
using namespace phoeg;

void describe(Graph& g, const char* name) {
  cout << "vertex set: ";
  print_vertices(g, name);
  cout << endl;

  cout << "edge set: ";
  print_edges(g, name);
  cout << endl;

  cout << "incident edges: " << endl;
  print_graph(g, name);
  cout << endl;

  cout << "NumCol: " << numCol(g) << endl;
}

int main(int, char*[]) {
  enum { A, B, C, D, E, F, N };
  const char* name = "ABCDEF";

  Graph g(N);
  
  add_edge(B, C, g);
  add_edge(B, F, g);
  add_edge(C, A, g);
  add_edge(D, E, g);
  add_edge(F, A, g);

  describe(g, name);

  Graph g1(g);

  add_edge(A, D, g1);

  describe(g1, name);

  Graph g2 = contract(A, D, g);

  describe(g2, name);
  
  return 0;
}
