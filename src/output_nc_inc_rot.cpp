#include <iostream>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"
#include "nauty_interface.hpp"

/* C++11 range iterator test. We should definitely augment boost with those. */
class VertexIterator {
public:
  VertexIterator(const phoeg::Graph& g) {
    boost::tie(b, e) = boost::vertices(g);
  }

  phoeg::vertex_iter begin() {
    return b;
  }

  phoeg::vertex_iter end() {
    return e;
  }
  
  phoeg::vertex_iter b, e;
};

void list_numcol_dec_rot(phoeg::Graph& g) {
  phoeg::vertex u;
  phoeg::adj_iter v, v_end;
  phoeg::vertex w;
  long numcol = phoeg::numCol(g);
  long numcol_rot;
  std::string sig;
  /* For each rotation center u. */
  for (auto u : VertexIterator(g)) {
    /* For each v adjacent to u. */
    for (boost::tie(v, v_end) = boost::adjacent_vertices(u, g);
         v != v_end; ++v) {
      /* For each w non-adjacent to u
         (there's no built-in iterator for that yet).*/
      for (auto w : VertexIterator(g)) {
        if (edge(u, w, g).second)
          continue;
        /* Apply the move. */
        boost::remove_edge(u, *v, g);
        boost::add_edge(u, w, g);
        numcol_rot = phoeg::numCol(g);
        if (numcol_rot < numcol) {
          /* We've got a NumCol decreasing move. List it. */
          sig = phoeg::convertToGraph6(phoeg::cannonForm(g));
          std::cout
            << "\\item \\([" << *v << " \\ot " << u
            << " \\to " << w << "] : "
            << sig << " : "
            << numcol << " \\to " << numcol_rot << "\\)" << std::endl;
        }
        /* Revert the move. */
        boost::remove_edge(u, w, g);
        boost::add_edge(u, *v, g);
      }
    }
  }
}


std::string verb(const std::string& s) {
  return "\\verb!" + s + "!";
}

void outputGraph(const std::string& sig) {
  phoeg::Graph g = phoeg::convertFromGraph6(sig);
  std::cout
    << std::endl
    << verb(sig) << std::endl
    << "\\begin{itemize}" << std::endl;
  list_numcol_dec_rot(g);
  std::cout
    << "\\end{itemize}" << std::endl;
}

int main(int argc, const char* argv[]) {
  std::cout
    << "\\documentclass[11pt,pdftex]{article}" << std::endl
    << "\\usepackage{amssymb,amsmath}" << std::endl
    << "\\usepackage[utf8]{inputenc}" << std::endl
    << "\\usepackage{latexsym, graphicx, epstopdf}" << std::endl
    << "\\newcommand{\\ot}{\\leftarrow}"
    << "\\begin{document}" << std::endl
    << std::endl;

  for (std::string line; std::getline(std::cin, line);)
    outputGraph(line);
  
  std::cout << "\\end{document}" << std::endl;
}
