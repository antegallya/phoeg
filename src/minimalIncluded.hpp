#ifndef MINIMAL_INCLUDED_H
#define MINIMAL_INCLUDED_H

#include <set>
#include "graph.hpp"
#include "graph6.hpp"
#include "morphisms.hpp"

namespace phoeg {

    /* Compares graphs by number of vertices, then by number of edges. */
    struct Graph_less {
        template <class Graph>
        bool operator()(const Graph & G1, const Graph & G2) const
        {
            return (order(G1) < order(G2)) ||
                   (order(G1) == order(G2) &&
                    numEdges(G1) < numEdges(G2));
        }
    };

    template <class Graph>
    bool sieve(std::multiset<Graph, Graph_less> & P, const Graph & G,
               bool (* subgraphCheck)(const Graph &, const Graph &))
    {
        auto it = P.begin();
        auto end_it = P.end();
        int n = order(G);
        int m = numEdges(G);

        /* Test if G is a supergraph of some graph with less vertices. */
        for (; it != end_it && order(*it) < n; it++) {
            if (subgraphCheck(*it, G)) {
                return false; /* G isn't a prime graph for sure ! */
            }
        }

        /* Test if G is a supergraph of some graph with the same number of
         * vertices but less edges. */
        for (; it != end_it && order(*it) == n && numEdges(g) < m; it++)
            if (subgraphCheck(*it, G)) {
                return false; /* G isn't a prime graph for sure ! */
            }

        /* Now, we'll check with the graphs with same number of vertices and
         * edges. Maybe G is isomorphic to some graph in P. */
        for (; it != end_it && order(g) == n && numEdges(g) == m; it++) {
            /* It would be faster to check for isomorphism here. Also, it might
             * be a precondition that we're never checking for isomorphic
             * graphs. */
            if (subgraphCheck(*it, G)) {
                return false; /* G isn't a prime graph for sure ! */
            }
        }

        /* Finally, we'll treat graphs bigger than G -- or more generally those
         * that we know cannot be subgraphs of G -- and test if G is a subgraph
         * of those -- as they can't be subgraphs of G. */
        while (it != end_it) {
            if (subgraphCheck(G, *it)) {
                it = P.erase(it);
            }
            else {
                ++it;
            }
        }

        /* If G makes it through here, then G is prime with all the graphs from
         * P. */
        P.insert(G);
        return true;
    }

    /* Returns the minimal included graphs stored in an iterable collection
     * coll. The graphs have to be Boost graphs stored with adjacency lists.
     * By default, checks for subgraph isomorphisms (induced subgraphs).  If
     * iso is set to false, checks for subgraph monomorphisms. Number of graph
     * comparisons in O(n*k), where n is the number of graphs and k is the size
     * of the answer. */
    template <class Graph, class T>
    std::multiset<Graph, Graph_less> minimalIncluded(
        const T & coll, bool iso = true)
    {
        auto subgraphCheck = iso ? subgraphIso<Graph> : subgraphMono<Graph>;
        std::multiset<Graph, Graph_less> P;

        for (const auto & it : coll) {
            sieve(P, it, subgraphCheck);
        }
        return P;
    }
}

#endif
