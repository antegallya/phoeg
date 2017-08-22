#ifndef MINIMAL_INCLUDED_H
#define MINIMAL_INCLUDED_H

#include <set>
#include "graph.hpp"
#include "graph6.hpp"
#include "morphisms.hpp"

/* Compares graphs by number of vertices, then by number of edges. */
struct Graph_less {
    template <class Graph>
    bool operator()(const Graph & G1, const Graph & G2) const
    {
        return (phoeg::order(G1) < phoeg::order(G2)) ||
               (phoeg::order(G1) == phoeg::order(G2) &&
                phoeg::numEdges(G1) < phoeg::numEdges(G2));
    }
};

/* Inspired from the same method in Graphedron xtraMinimalInclusion.cpp file. */
template <class Graph>
bool sieve(std::multiset<Graph, Graph_less> & P, const Graph & G,
           bool (* subgraphCheck)(const Graph &, const Graph &))
{
    auto it = P.begin();
    auto end_it = P.end();
    int n = phoeg::order(G);
    int m = phoeg::numEdges(G);

    /*  Tests if G is a supergraph of some graph with less vertices
        and less edges. */
    for (; it != end_it && phoeg::order(*it) <= n && phoeg::numEdges(*it) <= m
            ; ++it) {
        if (subgraphCheck(*it, G)) {
            return false;
        }
    }

    /*  We'll then treat graphs bigger than G -- or more generally those
        that we know cannot be subgraphs of G -- and test if G is a subgraph
        of those -- as they can't be subgraphs of G. */
    while (it != end_it) {
        if (subgraphCheck(G, *it)) {
            P.erase(it++);
        } else {
            ++it;
        }
    }

    /*  If G makes it through here, then G is prime with all the graphs
        from P. */
    P.insert(G);
    return true;
}

/*  Returns the minimal included graphs stored in an iterable collection coll.
    The graphs have to be Boost graphs stored with adjacency lists.
    By default, checks for subgraph isomorphisms (induced subgraphs).
    If iso is set to false, checks for subgraph monomorphisms. Number of graph
    comparisons in O(n*k), where n is the number of graphs and k is the size of
    the answer. */
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

#endif
