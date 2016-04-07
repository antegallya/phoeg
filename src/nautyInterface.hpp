#include <cstdio>
#include <string>
#include "nauty.h"
#include "graph.hpp"

//TODO : impossible to include graph.hpp because it conflicts with the graph type in nauty

//using namespace std;

/**
 * Computes the position of the first character encoding the adjacency matrix
 * in a graph6 format
 * @param n the order of the graph
 * @return the first character encoding the adjacency matrix
 */
int getStart(int n)
{
    if (n > 258047)
    {
        return 9;
    }
    else if (n > 62)
    {
        return 5;
    }
    else
    {
        return 1;
    }
}

/**
 * Fills the array with the edges of the graph given with the graph6 format
 * @param graph6 the graph that should be put in array<Plug>(neosnippet_expand)
 * @param n the order of the graph
 * @param m the size of the graph
 * @param array the array to fill
 */
void fillArray(const std::string & graph6, int n, int m, graph *array)
{
    int start = getStart(n);
    if (graph6.size() > 0)
    {
        long p = start;
        char v = graph6[p]-63;
        int l = 5;
        for (long i = 1; i < n; i++)
        {
            for (long j = 0; j < i; j++)
            {
                if (l < 0)
                {
                    l = 5;
                    p++;
                    v = graph6[p]-63;
                }
                //We get the value of the bit in position l
                if ((v % (1<<(l+1)))>>l)
                {
                    ADDONEEDGE(array, i, j, m);
                }
                l--;
            }
        }
    }
}

/**
 * Convert a graph with the same order as the graph in graph6
 * to the graph6 format.
 * @param graph6 a graph with same order as array
 * @param n the order of the graph
 * @param m the size of the graph
 * @param array the graph to convert
 * @return the graph with the graph6 format
 */
std::string makeSig(const std::string& graph6, int n, int m, graph* array)
{
    int start = getStart(n);
    std::string res = graph6.substr(0,start);
    int l = 0;
    char r = 0;
    set *row;
    //We skip the first vertex since we want the inferior
    //half of the adjacency matrix
    for (int i = 1; i < n ; ++i)
    {
        row = GRAPHROW(array, i, m);
        for (int j = 0; j < i; ++j)
        {
            if (l == 6)
            {
                res += (char) r+63;
                l = 0;
                r = 0;
            }
            //We add a bit if there is an edge
            bool isEdge = ISELEMENT(row, j);
            r = (r << 1) + (isEdge ? 1 : 0);
            l++;
        }
    }
    //We might have to add 0's at the end of the last byte
    //if it wasn't long enough
    if (l <= 6)
    {
        r = r << (6-l);
        res += (char) r+63;
    }
    return res;
}

/**
 * Returns the canonical form of the given graph with n vertices and m edges
 * @param sig the graph with graph6 format
 * @param n the order of the graph
 * @param m the size of the graph
 * @return the canonical form of the graph
 */
std::string cannonForm(std::string sig, int n, int m)
{
    int lab[n], ptn[n], orbit[n];
    static DEFAULTOPTIONS_GRAPH(options);
    options.getcanon = 1;
    statsblk stats;
    graph array[n*m], canon[n*m];
    EMPTYGRAPH(array, m, n);
    fillArray(sig, n, m, array);
    densenauty(array, lab, ptn, orbit, &options, &stats, m, n, canon);
    return makeSig(sig, n, m, canon);
}
