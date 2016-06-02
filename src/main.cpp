#include <iostream>
#include <iomanip>
#include <cmath>
#include "Minisat/Solver.hpp"
#include <boost/timer/timer.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"
#include "coloringProblem.hpp"
#include "graphFeatures.hpp"

using namespace std;
using namespace boost;

/**
* Test Method
*/
int main(int argc, char* argv[])
{
    string line;
    vector<phoeg::Graph> graphs;
    int n;
    int chromatic_phoeg;
    int chromatic_sat;
    timer::cpu_timer timer_phoeg;
    timer_phoeg.stop();
    timer::cpu_timer timer_sat;
    timer_sat.stop();

    cerr << "[INFO] Compute invariants values..." << endl;
    for (n = 0; getline(cin, line); ++n)
    {
        phoeg::Graph g = phoeg::convertFromGraph6(line);

        // PHOEG computation
        timer_phoeg.resume();
        chromatic_phoeg = phoeg::chromaticNumber(g);
        timer_phoeg.stop();

        // SAT computation
        timer_sat.resume();
        chromatic_sat = chromaticNumber(g, NULL, "");
        timer_sat.stop();

        if(chromatic_phoeg != chromatic_sat) {
            cerr << "[ERROR] Not the same values !"
                 << " PHOEG : " << chromatic_phoeg
                 << ", SAT : " << chromatic_sat << endl;
            return 1;
        }
    }

    cerr << "PHOEG : " << timer_phoeg.format()
         << "SAT   : " << timer_sat.format()
         << "[INFO] Done for " << n << " graphs." << endl;

    return 0;
}

/* vim: set sw=4: */
