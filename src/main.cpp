#include <iostream>
#include <iomanip>
#include <cmath>
#include "Minisat/Solver.hpp"
#include <boost/timer/timer.hpp>
#include <boost/graph/graph_utility.hpp>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"
#include "coloringProblem.hpp"
#include "graphFeatures.hpp"
#include "graphGeneration.hpp"

using namespace std;
using namespace boost;

int bad_instances()
{
    int n;
    int chromatic_phoeg;
    int chromatic_sat;
    timer::cpu_timer timer_phoeg;
    timer_phoeg.stop();
    timer::cpu_timer timer_sat;
    timer_sat.stop();
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
        timer_phoeg.resume();
        chromatic_phoeg = phoeg::chromaticNumber(g);
        timer_phoeg.stop();
        timer.stop();
        cerr << "PHOEG: " << timer.format();

        // SAT computation
        timer.start();
        timer_sat.resume();
        chromatic_sat = sat::chromaticNumber(g);
        timer_sat.stop();
        timer.stop();
        cerr << "SAT: " << timer.format();

        if(chromatic_phoeg != chromatic_sat) {
            cerr << "[ERROR] Not the same values !"
                 << " PHOEG: " << chromatic_phoeg
                 << ", SAT: " << chromatic_sat << endl;
            return 1;
        }
        cerr << endl;
    }

    cerr << "Cumul Time PHOEG: " << timer_phoeg.format()
         << "Cumul Time SAT  : " << timer_sat.format()
         << "[INFO] Done for " << n << " graphs." << endl;

    return 0;
}

int timings()
{
    string line;
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
        cerr << n + 1 << " : g6: " << phoeg::convertToGraph6(g)
             << ", n = " << num_vertices(g)
             << ", m = " << num_edges(g) << endl;

        // PHOEG computation
        timer_phoeg.resume();
        chromatic_phoeg = phoeg::chromaticNumber(g);
        timer_phoeg.stop();

        // SAT computation
        timer_sat.resume();
        chromatic_sat = sat::chromaticNumber(g);
        timer_sat.stop();

        if(chromatic_phoeg != chromatic_sat) {
            cerr << "[ERROR] Not the same values !"
                 << " PHOEG: " << chromatic_phoeg
                 << ", SAT: " << chromatic_sat << endl;
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
    return bad_instances();
}

/* vim: set sw=4: */
