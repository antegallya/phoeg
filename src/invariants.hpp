#ifndef INVARIANTS_IMPL_CPP
#define INVARIANTS_IMPL_CPP

#include "invariants.hpp"

#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <deque>

#define INF std::numeric_limits<long>::max()

using namespace boost;

typedef std::vector<std::vector<long> > dMatrix;

/**
 * Return the order of the graph g.
 * i.e., the number of nodes of g.
 */
template<class Graph>
long order(const Graph & g)
{
    return num_vertices(g);
}

/**
 * Return the size of the graph g.
 * i.e., the number of edges of g.
 */
template<class Graph>
long numEdges(const Graph & g)
{
    return num_edges(g);
}

/**
 * Return the distance matrix computed using
 * Floyd-Warshall algorithm.
 */
template<class Graph>
dMatrix floydWarshall(const Graph & g)
{
    long n = order(g);
    dMatrix matrix;
    for (long i = 0; i < n; ++i)
    {
        std::vector<long> line;
        for (long j = 0; j < n; ++j)
        {
            if (i == j)
            {
                line.push_back(0);
            }
            else if (edge(i,j,g).second)
            {
                line.push_back(1);
            }
            else
            {
                line.push_back(INF);
            }
        }
        matrix.push_back(line);
    }
    for (long k = 0; k < n; ++k)
    {
        for (long i = 0; i < n; ++i)
        {
            for (long j = 0; j < n; ++j)
            {
                //If both distances are not infinite
                if (matrix[i][k] != INF && matrix[k][j] != INF)
                {
                    //If d(i,j) is infinite or distance bigger than d(i,k)+d(k,j)
                    if (matrix[i][j] == INF || matrix[i][j] > matrix[i][k] + matrix[k][j])
                    {
                        matrix[i][j] = matrix[i][k] + matrix[k][j];
                    }
                }
            }
        }
    }
    return matrix;
}

/**
 * Computes the distance matrix of graph g
 */
template <class Graph>
dMatrix distanceMatrix(const Graph & g)
{
    return floydWarshall(g);
}

/**
 * Returns the diameter of the graph g.
 * i.e., the maximal distance between two nodes.
 * If the graph is not connected, returns INF
 */
template <class Graph>
long diameter(const Graph & g)
{
    dMatrix dist = distanceMatrix(g);
    long n = order(g);
    long max = -1;
    for (long i = 0; i < n; ++i)
    {
        //We need to consider the graph with n = 1
        //thus, j = i
        for (long j = i; j < n; ++j)
        {
            if (max < dist[i][j])
            {
                max = dist[i][j];
            }
        }
    }
    return max;
}

/**
 * Returns the radius of the graph g.
 * i.e., the minimum of the largest distances
 * If the graph is not connected, returns INF
 */
template <class Graph>
long radius(const Graph & g)
{
    dMatrix mat = distanceMatrix(g);
    long n = order(g);
    long rad = INF;
    for (long i = 0; i < n; ++i)
    {
        long max = -1;
        for (long j = 0; j < n; ++j)
        {
            if (mat[i][j] > max)
            {
                max = mat[i][j];
            }
        }
        if (max < rad)
        {
            rad = max;
        }
    }
    return rad;
}

/**
 * Returns the list of degrees of the nodes
 * sorted in increasing order
 */
template<class Graph>
std::vector<long> listDegrees(const Graph & g)
{
    std::vector<long> list(order(g), 0);
    typedef typename graph_traits<Graph>::edge_iterator eiter;
    std::pair<eiter, eiter> ep;
    for (ep = edges(g); ep.first != ep.second; ++ep.first)
    {
        int u = source(*ep.first,g), v = target(*ep.first,g);
        list[u]++;
        list[v]++;
    }
    make_heap(list.begin(), list.end());
    sort_heap(list.begin(), list.end());
    return list;
}

/**
 * Returns the maximal degree of the graph g.
 * i.e., the maximal number of neighbors in the graph.
 */
template<class Graph>
long maxDegree(const Graph & g)
{
    std::vector<long> degrees = listDegrees(g);
    if (degrees.size() > 0)
    {
        return degrees[degrees.size()-1];
    }
    else
    {
        return 0;
    }
}

/**
 * Returns the minimal degree of the graph g.
 * i.e., the minimal number of neighbors in the graph.
 */
template<class Graph>
long minDegree(const Graph & g)
{
    std::vector<long> degrees = listDegrees(g);
    if (degrees.size() > 0)
    {
        return degrees[0];
    }
    else
    {
        return 0;
    }
}

/**
 * Tests weither the graph g is connected.
 */
template<class Graph>
bool isConnected(const Graph & g)
{
    std::vector<int> comps(order(g));
    long num = connected_components(g, &comps[0]);
    return num < 2;
}

/**
 * Tests weither the graph g is a tree.
 * i.e., connected and n-1 edges.
 */
template<class Graph>
bool isTree(const Graph & g)
{
    return isConnected(g) && (numEdges(g) == order(g)-1);
}

//TODO change it for contract in graph class
template <class Graph>
Graph contract(const Graph & g, typename graph_traits<Graph>::vertex_descriptor u, typename graph_traits<Graph>::vertex_descriptor v)
{
    Graph g1(order(g)-1);
    if (u > v)
    {
        typename graph_traits<Graph>::vertex_descriptor tmp = u;
        u = v;
        v = tmp;
    }
    for (long i = 0; i < order(g); i++)
    {
        for (long j = i+1; j < order(g); j++)
        {
            if (edge(i,j,g).second)
            {
                if (j == v && i != u)
                {
                    add_edge(i,u,g1);
                }
                else if (i == v)
                {
                    add_edge(u,j-1,g1);
                }
                else if (j != v)
                {
                    long a = i;
                    long b = j;
                    if (i > v)
                    {
                        a--;
                    }
                    if (j > v)
                    {
                        b--;
                    }
                    add_edge(a,b,g1);
                }
            }
        }
    }
    return g1;
}

/**
 * Computes the number of non-equivalent
 * colorings of the graph g.
 */
//template<class Graph>
//long numCol(const Graph & g)
//{
    //long n = order(g);
    //if (n == 1 || minDegree(g) == n-1)
    //{
        //return 1;
    //}
    //typedef typename graph_traits<Graph>::vertex_iterator vertex_iter;
    //typedef std::pair<vertex_iter, vertex_iter> p_vertex_iter;
    //typedef typename graph_traits<Graph>::vertex_descriptor vertex;
    //vertex u,v;
    //bool nonEdgeFound = false;
    //for (p_vertex_iter vp = vertices(g); vp.first != vp.second && !nonEdgeFound; ++vp.first)
    //{
        //for (vertex_iter w = vp.first+1; w != vp.second && !nonEdgeFound; ++w)
        //{
            //if (!edge(*vp.first, *w, g).second)
            //{
                //u = *vp.first;
                //v = *w;
                //nonEdgeFound = true;
            //}
        //}
    //}
    //Graph g1 = contract_impl(g, u, v);
    //long num = numCol(g1);
    //Graph g2(g);
    //add_edge(u,v,g2);
    //num += numCol(g2);
    //return num;
//}

/**
 * Checks weither a graph g is planar.
 * i.e., if it can be drawn without
 * crossing edges.
 */
template <class Graph>
bool isPlanar(const Graph & g)
{
    //We need to convert g into an adjacency_list because
    //the function of boost does not work correctly
    //on adjacency_matrix
    typedef boost::adjacency_list<vecS, vecS, undirectedS> AdjList;
    typedef typename graph_traits<Graph>::edge_iterator eiter;
    AdjList h(order(g));
    std::pair<eiter, eiter> ep;
    for (ep = edges(g); ep.first != ep.second; ++ep.first)
    {
        int u = source(*ep.first,g), v = target(*ep.first,g);
        add_edge(u,v,h);
    }
    return boyer_myrvold_planarity_test(h);
}

template <class Graph>
std::vector<long> listEccentricities(const Graph & g)
{
    dMatrix dist = distanceMatrix(g);
    long n = order(g);
    std::vector<long> res(n);
    for (long i = 0; i < n; i++)
    {
        long m = 0;
        for (long j = 0; j < n; j++)
        {
            if (m < dist[i][j])
            {
                m = dist[i][j];
            }
        }
        res[i] = m;
    }
    make_heap(res.begin(), res.end());
    sort_heap(res.begin(), res.end());
    return res;
}

/**
 * Compute the eccentric connectivity of a graph g.
 * i.e., the sum for every node v of the products
 * between the eccentricity of v and its degree.
 * The graph needs to be connected.
 */
template <class Graph>
long eccentricConnectivity(const Graph & g)
{
    long res = 0, n = order(g);
    dMatrix dist = distanceMatrix(g);
    for (long i = 0; i < n; i++)
    {
        long ecc = 0, deg = 0;
        for (long j = 0; j < n; j++)
        {
            if (edge(i,j,g).second)
            {
                deg++;
            }
            if (dist[i][j] > ecc)
            {
                ecc = dist[i][j];
            }
        }
        res += ecc * deg;
    }
    return res;
}

#endif
