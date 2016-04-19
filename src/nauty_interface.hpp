#include <cstdio>
#include <string>
#include "nauty.h"
#include "graph.hpp"

namespace phoeg
{
    namespace detail
    {
        /**
         * Fill the array by adding the edges of the graph g
         * @param g the graph to put in array
         * @param n the order of g
         * @param m the size of g
         * @param array the array to fill
         */
        void fillArrayGraph(const phoeg::Graph & g, int n, int m, graph *array)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    if (edge(i,j,g).second)
                    {
                        ADDONEEDGE(array, i, j, m);
                    }
                }
            }
        }

        /**
         * Add the edges of the graph stored in array into the
         * graph g
         * @param g a graph with same order as array
         * @param n the order of the graph
         * @param m the size of the graph
         * @param array the graph to convert
         */
        void fillGraph(phoeg::Graph& g, int n, int m, graph* array)
        {
            set *row;
            //We skip the first vertex since we want the inferior
            //half of the adjacency matrix
            for (int i = 1; i < n ; ++i)
            {
                row = GRAPHROW(array, i, m);
                for (int j = 0; j < i; ++j)
                {
                    if (ISELEMENT(row, j))
                    {
                        add_edge(i,j,g);
                    }
                }
            }
        }
    }

    /**
     * Returns the canonical form of the given graph with n vertices and m edges
     * @param g the graph
     * @return the canonical form of the graph
     */
    phoeg::Graph cannonForm(const phoeg::Graph& g)
    {
        int n = num_vertices(g), m = num_edges(g);
        int lab[n], ptn[n], orbit[n];
        static DEFAULTOPTIONS_GRAPH(options);
        options.getcanon = 1;
        statsblk stats;
        graph array[n*m], canon[n*m];
        EMPTYGRAPH(array, m, n);
        detail::fillArrayGraph(g, n, m, array);
        densenauty(array, lab, ptn, orbit, &options, &stats, m, n, canon);
        phoeg::Graph ng(n);
        detail::fillGraph(ng, n, m, canon);
        return ng;
    }
}
