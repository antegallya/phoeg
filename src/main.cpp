#include <iostream>
#include <iomanip>
#include <cmath>
#include <functional>
#include "Minisat/Solver.hpp"
#include <boost/timer/timer.hpp>
#include <boost/graph/graph_utility.hpp>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"
#include "coloringProblem.hpp"
#include "minimumVertexCoverProblem.hpp"
#include "graphFeatures.hpp"
#include "graphGeneration.hpp"

using namespace std;
using namespace boost;

typedef function<long(const phoeg::Graph&)> invariant;
typedef long(*invariant_c)(const phoeg::Graph&);

int bad_instances(invariant phoeg_method, invariant sat_method)
{
    int n;
    int inv_phoeg;
    int inv_sat;
    timer::cpu_timer timer;
    timer.stop();

    cerr << "[INFO] Compute invariants values..." << endl;
    for (n = 0; n < 10; ++n)
    {
        phoeg::Graph g = generRandomGraph(21);
        cerr << n + 1 << " : g6: " << phoeg::convertToGraph6(g)
             << ", n = " << num_vertices(g)
             << ", m = " << num_edges(g) << endl;

        // PHOEG computation
        timer.start();
        inv_phoeg = phoeg_method(g);
        timer.stop();
        cerr << "PHOEG: " << timer.format();

        // SAT computation
        timer.start();
        inv_sat = sat_method(g);
        timer.stop();
        cerr << "SAT: " << timer.format();

        if(inv_phoeg != inv_sat) {
            cerr << "[ERROR] Not the same values !"
                 << " PHOEG: " << inv_phoeg
                 << ", SAT: " << inv_sat << endl;
            return 1;
        }
        cerr << endl;
    }

    cerr << "[INFO] Done for " << n << " graphs." << endl;

    return 0;
}

int timings(invariant phoeg_method, invariant sat_method)
{
    string line;
    int n;
    int inv_phoeg;
    int inv_sat;
    timer::cpu_timer timer_phoeg;
    timer_phoeg.stop();
    timer::cpu_timer timer_sat;
    timer_sat.stop();

    cerr << "[INFO] Compute invariants values..." << endl;
    for (n = 0; getline(cin, line); ++n)
    {
        phoeg::Graph g = phoeg::convertFromGraph6(line);
        cerr << n + 1 << " : g6: " << phoeg::convertToGraph6(g)
             << ", n = " << num_vertices(g)
             << ", m = " << num_edges(g) << endl;

        // PHOEG computation
        timer_phoeg.resume();
        inv_phoeg = phoeg_method(g);
        timer_phoeg.stop();

        // SAT computation
        timer_sat.resume();
        inv_sat = sat_method(g);
        timer_sat.stop();

        if(inv_phoeg != inv_sat) {
            cerr << "[ERROR] Not the same values !"
                 << " PHOEG: " << inv_phoeg
                 << ", SAT: " << inv_sat << endl;
            return 1;
        }
    }

    cerr << "Cumul Time PHOEG: " << timer_phoeg.format()
         << "Cumul Time SAT  : " << timer_sat.format()
         << "[INFO] Done for " << n << " graphs." << endl;

    return 0;
}

/**
* Test Method
*/
int main(int argc, char* argv[])
{
    return timings(phoeg::minVertexCover<phoeg::Graph>,
                   sat::minimumVertexCover);
}

/* vim: set sw=4: */
