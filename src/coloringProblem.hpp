#include <string>
#include "graph.hpp"
#include "invariants.hpp"

namespace sat
{

/**
* Coding of proposals (variables) for the SAT.
*/
inline int prop(int n, int s, int c)
{
    return (n * (n + 1) - (n - c + 1) * (n - c + 2)) / 2 + s - c;
}

/**
* Calculate the chromatic number of a graph with a SAT problem.
*/
bool is_k_colorable(phoeg::Graph& g, int k)
{
    int n = num_vertices(g);
    int varNumber = (n * (n + 1) - (n - k) * (n - k + 1)) / 2;

    Solver solver;
    vec<Lit> lits;

    //Creation of variables
    for(int i = 0; i < varNumber; i++)
        solver.newVar();

    // Constraint neighboring
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            if (edge(i-1, j-1, g).second)
                for(int c = 1 ; c <= std::min(std::min(i, j), k); c++)
                    solver.addBinary(~Lit(prop(n, i, c)),
                                     ~Lit(prop(n, j, c)));

    // Constraint existence : One color by node
    for(int i = 1; i <= n; i++)
    {
        lits.clear();
        for(int c = 1 ; c <= std::min(k, i); c++)
            lits.push(Lit(prop(n, i, c)));
        solver.addClause(lits);
    }

    return solver.solve(); //Solve the sat problem
}

/**
* Call the main method (SAT method) with a good approximation.
*/
int chromaticNumber(phoeg::Graph& g)
{
    /* Approximate the chromatic number and start from there. */
    int k = phoeg::detail::seq_colors(g);

    while (k > 1 && is_k_colorable(g, k - 1))
        --k;

    return k;
}

/**
* Call the main method (SAT method) with a good approximation on graph6 format.
*/
int chromaticNumber(std::string graph6)
{
    phoeg::Graph g = phoeg::convertFromGraph6(graph6);
    return chromaticNumber(g);
}

}
