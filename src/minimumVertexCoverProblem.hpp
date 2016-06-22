#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include "graph.hpp"
#include "invariants.hpp"
#include "graphFeatures.hpp"
#include <cmath>

#include <list>
#include <iostream>
#include <unistd.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

namespace sat
{

namespace mvc_detail
{
    inline int prop(int n, int i, int j)
    {
        /*     i
         *   0 1 2
         * j 3 4 5
         *   6 7 8 */
        return j * n + i;
    }

    /**
    * Compute the Minimum Vertex Cover with SAT
    */
    bool is_k_vertex_coverable(const phoeg::Graph& g, int k)
    {
        int n = num_vertices(g);
        int varNumber = n * k;

        Solver solver;
        vec<Lit> lits;

        //Creation of proposals
        for(int i = 0 ; i < varNumber; i++)
        {
            solver.newVar();
        }


        //Constraint : Each vertice can belong in the cover
        for(int j=0; j<k; j++)
        {
            lits.clear();
            for(int i=0; i < n; i++)
            {
                lits.push(Lit(prop(n, i, j)));
            }
            solver.addClause(lits);
        }

        int p;
        //Constraint : Each vertice belonging to the cover must be different
        for(int m=0; m < n; m++)
        {
            for(int i=1; i<k; i++)
            {
                p=i;
                for(int q=p+1; q<=k; q++)//Create binary clause
                {
                    lits.clear();
                    lits.push(~Lit(prop(n, m, p-1)));
                    lits.push(~Lit(prop(n, m, q-1)));
                    solver.addClause(lits);
                }
            }
        }
        //Constraint : Each set must be covered by only one vertice
        for(int m=1; m<=k; m++)
        {
            for(int i=1;i < n;i++)
            {
                p=i;
                for(int q=p+1; q <= n; q++)//Create binary clause
                {
                    lits.clear();
                    lits.push(~Lit(prop(n, p-1, m-1)));
                    lits.push(~Lit(prop(n, q-1, m-1)));
                    solver.addClause(lits);
                }
            }
        }

        //Constraint : Each edge must be covered
        for(int i=0;i< n;i++)
        {
            for(int j=i; j < n; j++)
            {
                if(edge(i, j, g).second)
                {
                    lits.clear();
                    for(int l=0; l<k; l++)
                    {
                        lits.push(Lit(prop(n, i, l)));
                        lits.push(Lit(prop(n, j, l)));
                    }
                    solver.addClause(lits);
                }
            }
        }

        return solver.solve();//Solve the sat problem
    }

    /**
    *Computes a Vertex Cover by minimizing his size
    *The heuristic provides an approximation for the SAT method
    * (Source = http://www.geeksforgeeks.org/vertex-cover-problem-set-1-introduction-approximate-algorithm-2/)
    */
    int approxMinVertexCover(const phoeg::Graph& g)
    {
        using namespace boost;

        std::list<int> *adj;
        int n = num_vertices(g);
        adj = new std::list<int>[n];

        std::vector<std::vector<int> > tab;

        tab = neighborsVector(g);//Creating an array containing the neighbors for each vertice

        typedef typename graph_traits<phoeg::Graph>::edge_iterator eiter;
        std::pair<eiter, eiter> ep;

        for (ep = edges(g); ep.first != ep.second; ++ep.first)
        {
            int u = source(*ep.first,g), v = target(*ep.first,g);
            adj[u].push_back(v); // Add w to v’s list.
            adj[v].push_back(u); // Since the graph is undirected
        }


        // Initialize all vertices as not visited.
        bool visited[n];
        for (int i=0; i < n; i++)
        visited[i] = false;

        std::list<int>::iterator i;

        // Consider all edges one by one
        for (int u=0; u < n; u++)
        {
            // An edge is only picked when both visited[u] and visited[v] are false
            if (visited[u] == false)
            {
                // Go through all adjacents of u and pick the first not
                // yet visited vertex (We are basically picking an edge
                // (u, v) from remaining edges.
                for (i= adj[u].begin(); i != adj[u].end(); ++i)
                {
                    int v = *i;
                    if (visited[v] == false)
                    {
                        // Add the vertices (u, v) to the result set.
                        // We make the vertex u and v visited so that
                        // all edges from/to them would be ignored
                        visited[v] = true;
                        visited[u] = true;
                        break;
                    }
                }
            }
        }
        int result = 0;
        // Compute the vertex cover
        for (int i=0; i < n; i++)
            if (visited[i])
            {
                result++;
            }
        return result;
    }
} // namespace detail

/**
* Call the main method (SAT method) with a good approximation.
*/
long minimumVertexCover(const phoeg::Graph& g)
{
    int k = mvc_detail::approxMinVertexCover(g);

    while (k > 0 && mvc_detail::is_k_vertex_coverable(g, k - 1))
        --k;

    return k;
}

}
