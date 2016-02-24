#include <iostream>
#include <boost/graph/graph_utility.hpp>
#include "graph.hpp"
#include "invariants.hpp"
#define BOOST_TEST_MODULE invariantsTests
#include <boost/test/included/unit_test.hpp>

using namespace boost;
using namespace std;
using namespace phoeg;

typedef pair<vertex_iter, vertex_iter> p_vertex_iter;

//Stable set
Graph g1(5);
//Disconnected graph
Graph g2(5);
//Tree
Graph g3(5);
//C5 with chord
Graph g4(5);
//Empty graph
Graph g5(1);
//K5
Graph g6(5);

void initGraphs()
{
    add_edge(0,1,g2);
    add_edge(1,2,g2);
    add_edge(2,0,g2);
    add_edge(3,4,g2);

    add_edge(0,1,g3);
    add_edge(0,2,g3);
    add_edge(1,3,g3);
    add_edge(1,4,g3);

    add_edge(0,1,g4);
    add_edge(1,2,g4);
    add_edge(2,3,g4);
    add_edge(3,4,g4);
    add_edge(4,0,g4);
    add_edge(1,3,g4);

    add_edge(0,1,g6);
    add_edge(0,2,g6);
    add_edge(0,3,g6);
    add_edge(0,4,g6);
    add_edge(1,2,g6);
    add_edge(1,3,g6);
    add_edge(1,4,g6);
    add_edge(2,3,g6);
    add_edge(2,4,g6);
    add_edge(3,4,g6);

}

void clearGraph(Graph & g)
{
    for (p_vertex_iter it = vertices(g); it.first != it.second; ++it.first)
    {
        clear_vertex(*it.first, g);
    }
}

void clearGraphs()
{
    clearGraph(g1);
    clearGraph(g2);
    clearGraph(g3);
    clearGraph(g4);
    clearGraph(g5);
    clearGraph(g6);
}

struct config
{
    config() {initGraphs();}
    ~config() {clearGraphs();}
};

BOOST_FIXTURE_TEST_SUITE(s, config)

BOOST_AUTO_TEST_CASE(orderTest)
{
    BOOST_CHECK_EQUAL(5, order(g1));
    BOOST_CHECK_EQUAL(5, order(g2));
    BOOST_CHECK_EQUAL(5, order(g3));
    BOOST_CHECK_EQUAL(5, order(g4));
    BOOST_CHECK_EQUAL(1, order(g5));
    BOOST_CHECK_EQUAL(5, order(g6));
}

BOOST_AUTO_TEST_CASE(numEdgesTest)
{
    BOOST_CHECK_EQUAL(0, numEdges(g1));
    BOOST_CHECK_EQUAL(4, numEdges(g2));
    BOOST_CHECK_EQUAL(4, numEdges(g3));
    BOOST_CHECK_EQUAL(6, numEdges(g4));
    BOOST_CHECK_EQUAL(0, numEdges(g5));
    BOOST_CHECK_EQUAL(10, numEdges(g6));
}

BOOST_AUTO_TEST_CASE(diameterTest)
{
    BOOST_CHECK_EQUAL(INF, diameter(g1));
    BOOST_CHECK_EQUAL(INF, diameter(g2));
    BOOST_CHECK_EQUAL(3, diameter(g3));
    BOOST_CHECK_EQUAL(2, diameter(g4));
    BOOST_CHECK_EQUAL(0, diameter(g5));
    BOOST_CHECK_EQUAL(1, diameter(g6));
}

BOOST_AUTO_TEST_CASE(radiusTest)
{
    BOOST_CHECK_EQUAL(INF, radius(g1));
    BOOST_CHECK_EQUAL(INF, radius(g2));
    BOOST_CHECK_EQUAL(2, radius(g3));
    BOOST_CHECK_EQUAL(2, radius(g4));
    BOOST_CHECK_EQUAL(0, radius(g5));
    BOOST_CHECK_EQUAL(1, radius(g6));
}

BOOST_AUTO_TEST_CASE(maxDegreeTest)
{
    BOOST_CHECK_EQUAL(0, maxDegree(g1));
    BOOST_CHECK_EQUAL(2, maxDegree(g2));
    BOOST_CHECK_EQUAL(3, maxDegree(g3));
    BOOST_CHECK_EQUAL(3, maxDegree(g4));
    BOOST_CHECK_EQUAL(0, maxDegree(g5));
    BOOST_CHECK_EQUAL(4, maxDegree(g6));
}

BOOST_AUTO_TEST_CASE(minDegreeTest)
{
    BOOST_CHECK_EQUAL(0, minDegree(g1));
    BOOST_CHECK_EQUAL(1, minDegree(g2));
    BOOST_CHECK_EQUAL(1, minDegree(g3));
    BOOST_CHECK_EQUAL(2, minDegree(g4));
    BOOST_CHECK_EQUAL(0, minDegree(g5));
    BOOST_CHECK_EQUAL(4, minDegree(g6));
}

BOOST_AUTO_TEST_CASE(isConnectedTest)
{
    BOOST_CHECK_EQUAL(false, isConnected(g1));
    BOOST_CHECK_EQUAL(false, isConnected(g2));
    BOOST_CHECK_EQUAL(true, isConnected(g3));
    BOOST_CHECK_EQUAL(true, isConnected(g4));
    BOOST_CHECK_EQUAL(true, isConnected(g5));
    BOOST_CHECK_EQUAL(true, isConnected(g6));
}

BOOST_AUTO_TEST_CASE(isTreeTest)
{
    BOOST_CHECK_EQUAL(false, isTree(g1));
    BOOST_CHECK_EQUAL(false, isTree(g2));
    BOOST_CHECK_EQUAL(true, isTree(g3));
    BOOST_CHECK_EQUAL(false, isTree(g4));
    BOOST_CHECK_EQUAL(true, isTree(g5));
    BOOST_CHECK_EQUAL(false, isTree(g6));
}

template<class type>
void checkEqualVector(vector<type> v, vector<type> w)
{
    BOOST_CHECK_EQUAL(v.size(), w.size());
    for (long i = 0; i < v.size(); ++i)
    {
        BOOST_CHECK_EQUAL(v[i], w[i]);
    }
}

void checkEqualMatrix(dMatrix m, dMatrix n)
{
    BOOST_CHECK_EQUAL(m.size(), n.size());
    for (long i = 0; i < m.size(); ++i)
    {
        checkEqualVector(m[i],n[i]);
    }
}

vector<long> initVector(long arr[], long size)
{
    vector<long> v(arr, arr+size);
    return v;
}

dMatrix initMatrix(long arr[], long width, long height)
{
    dMatrix mat(height);
    for (long i = 0; i < height; ++i)
    {
        vector<long> v(arr+(i*width), arr+((i+1)*width));
        mat[i] = v;
    }
    return mat;
}

BOOST_AUTO_TEST_CASE(listDegreeTest)
{
    long g1t[] = {0,0,0,0,0};
    checkEqualVector(initVector(g1t,5), listDegrees(g1));
    long g2t[] = {1,1,2,2,2};
    checkEqualVector(initVector(g2t,5), listDegrees(g2));
    long g3t[] = {1,1,1,2,3};
    checkEqualVector(initVector(g3t,5), listDegrees(g3));
    long g4t[] = {2,2,2,3,3};
    checkEqualVector(initVector(g4t,5), listDegrees(g4));
    vector<long> g5v(1,0);
    checkEqualVector(g5v, listDegrees(g5));
    long g6t[] = {4,4,4,4,4};
    checkEqualVector(initVector(g6t,5), listDegrees(g6));
}

BOOST_AUTO_TEST_CASE(distanceMatrixTest)
{
    long g1t[] = {0, INF, INF, INF, INF,
                  INF, 0, INF, INF, INF,
                  INF, INF, 0, INF, INF,
                  INF, INF, INF, 0, INF,
                  INF, INF, INF, INF, 0};
    checkEqualMatrix(initMatrix(g1t, 5, 5), distanceMatrix(g1));
    long g2t[] = {0, 1, 1, INF, INF,
                  1, 0, 1, INF, INF,
                  1, 1, 0, INF, INF,
                  INF, INF, INF, 0, 1,
                  INF, INF, INF, 1, 0};
    checkEqualMatrix(initMatrix(g2t, 5, 5), distanceMatrix(g2));
    long g3t[] = {0, 1, 1, 2, 2,
                  1, 0, 2, 1, 1,
                  1, 2, 0, 3, 3,
                  2, 1, 3, 0, 2,
                  2, 1, 3, 2, 0};
    checkEqualMatrix(initMatrix(g3t, 5, 5), distanceMatrix(g3));
    long g4t[] = {0, 1, 2, 2, 1,
                  1, 0, 1, 1, 2,
                  2, 1, 0, 1, 2,
                  2, 1, 1, 0, 1,
                  1, 2, 2, 1, 0};
    checkEqualMatrix(initMatrix(g4t, 5, 5), distanceMatrix(g4));
    long g5t[] = {0};
    checkEqualMatrix(initMatrix(g5t, 1, 1), distanceMatrix(g5));
    long g6t[] = {0, 1, 1, 1, 1,
                  1, 0, 1, 1, 1,
                  1, 1, 0, 1, 1,
                  1, 1, 1, 0, 1,
                  1, 1, 1, 1, 0};
    checkEqualMatrix(initMatrix(g6t, 5, 5), distanceMatrix(g6));
}

BOOST_AUTO_TEST_CASE(planarityTest)
{
    BOOST_CHECK_EQUAL(true, isPlanar(g1));
    BOOST_CHECK_EQUAL(true, isPlanar(g2));
    BOOST_CHECK_EQUAL(true, isPlanar(g3));
    BOOST_CHECK_EQUAL(true, isPlanar(g4));
    BOOST_CHECK_EQUAL(true, isPlanar(g5));
    BOOST_CHECK_EQUAL(false, isPlanar(g6));
}

//TODO correct contraction
BOOST_AUTO_TEST_CASE(numColTest)
{
    Graph g(2);
    BOOST_CHECK_EQUAL(2,numCol(g));
    BOOST_CHECK_EQUAL(52, numCol(g1));
    BOOST_CHECK_EQUAL(13, numCol(g2));
    BOOST_CHECK_EQUAL(15, numCol(g3));
    BOOST_CHECK_EQUAL(8, numCol(g4));
    BOOST_CHECK_EQUAL(1, numCol(g5));
}

BOOST_AUTO_TEST_SUITE_END()
