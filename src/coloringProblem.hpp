#include <string>
#include <algorithm>
#include "graph.hpp"
#include "invariants.hpp"
#include "Minisat/Solver.hpp"

namespace sat
{
namespace chroma_detail
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
    bool is_k_colorable(const phoeg::Graph& g, int k)
    {
        int i, j, c;
        int n = num_vertices(g);
        int varNumber = (n * (n + 1) - (n - k) * (n - k + 1)) / 2;

        Solver solver;
        vec<Lit> lits(k);

        //Creation of variables
        for(i = 0; i < varNumber; i++)
            solver.newVar();

        // Constraint neighboring
        for(i = 1; i <= n; i++)
            for(j = 1; j < i; j++)
                if (edge(i-1, j-1, g).second)
                    for(c = 1; c <= std::min(std::min(i, j), k); c++)
                        solver.addBinary(~Lit(prop(n, i, c)),
                                         ~Lit(prop(n, j, c)));

        // Constraint existence : One color by node
        for(i = 1; i <= n; i++)
        {
            lits.clear();
            for(c = 1; c <= std::min(k, i); c++)
                lits.push(Lit(prop(n, i, c)));
            solver.addClause(lits);
        }

        return solver.solve(); //Solve the sat problem
    }
}

/**
* Call the main method (SAT method) with a good approximation.
*/
long chromaticNumber(const phoeg::Graph& g)
{
    /* Approximate the chromatic number and start from there. */
    int k = phoeg::detail::seq_colors(g);

    while (k > 1 && chroma_detail::is_k_colorable(g, k - 1))
        --k;

    return k;
}

}
