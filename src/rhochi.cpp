#include <iostream>
#include <boost/graph/graph_utility.hpp>
#include "graph.hpp"
#include "graph6.hpp"
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
  const char* name = "petersen";

  Graph g = convertFromGraph6("IsP@OkWHG");

  describe(g, name);

  int k = 3;
  bool krho = phoeg::detail::is_k_colorable(g, k, true);
  cout << "Is " << k << " robust-colorable: " << krho << endl;
  cout << "Its numcol-" << k << " is:" << numColK(g, k) << endl;

  return 0;
}
