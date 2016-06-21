#ifndef GRAPHFEATURES_CPP
#define GRAPHFEATURES_CPP
#include <iostream>
#include <cmath>
#include "graph.hpp"

//Declaration of methods
std::vector<std::vector<int> > neighborsVector(phoeg::Graph& g);
/**
* Provides features on the graph
*/
void describe(phoeg::Graph& g, const char* name)
{
    std::cout << "Ensemble des sommets du graphe: ";
    print_vertices(g, name);
    std::cout << std::endl;

    std::cout << "Ensemble des arêtes du graphe: ";
    print_edges(g, name);
    std::cout << std::endl;

    std::cout << "Arètes incidentes: " << std::endl;
    print_graph(g, name);
    std::cout << std::endl;

    std::cout <<  " Nombre de sommets : " << num_vertices(g) << std::endl;
}


/**
* Check if two vertices are neighbors  => Not used (see neighborsTest())
*/
bool neighbors(phoeg::Graph& g, int i, int j) 
{
    using namespace boost;
    typedef typename graph_traits<phoeg::Graph>::edge_iterator eiter;
    std::pair<eiter, eiter> ep;

    for(ep = edges(g); ep.first != ep.second; ++ep.first)//Recovering each edge of the graph (u,v)
    {
        int u = source(*ep.first,g), v = target(*ep.first,g);
        if((u ==i && v==j) || (u==j && v ==i))
        {
            return 1;//i and j are neighbors
        }
    }
    return 0;//i and j are not neighbors
}
/**
* Check if two vertices are neighbors with a neighbors vector
*/
bool neighborsTest(int i, int j, std::vector<std::vector<int> > v)
{
    for(int k = 0; k < (int) v[i].size(); k++)
    {
        if(v[i][k] == j)
        {
            return 1;//i and j are neighbors
        }
    }
    return 0;//i and j are not neighbors
}

/**
* Print neighbors for each vertices
*/
void printNeighbors(std::vector<std::vector<int> > tab)
{
    for(int i=0; i < (int) tab.size(); i++)
    {
        for(int j =0 ; j < (int) tab[i].size(); j++)
        {
            std::cout << tab[i][j] << std::endl;
        }
    }
}

/**
* Print all edges for a graph
*/
void printEdges(phoeg::Graph& g)
{
    using namespace boost;
    typedef typename graph_traits<phoeg::Graph>::edge_iterator eiter;
    std::pair<eiter, eiter> ep;

    for (ep = edges(g); ep.first != ep.second; ++ep.first)//Recovering each edge of the graph (u,v)
    {
        int u = source(*ep.first,g), v = target(*ep.first,g);
        std::cout <<  " ( " << u << " , " << v << ")"  << std::endl;
    }
}

/**
* Return the edges number for a graph
*/
int edgesNumber(phoeg::Graph& g)
{
    using namespace boost;
    typedef typename graph_traits<phoeg::Graph>::edge_iterator eiter;
    std::pair<eiter, eiter> ep;
    int count=0;
    for (ep = edges(g); ep.first != ep.second; ++ep.first)//Recovering each edge of the graph (u,v)
    {
        count++;
    }
    return count;
}

/**
* Check if a graph is complete or not
*/
bool isCompleteGraph(phoeg::Graph& g)
{
    std::vector<std::vector<int> > tab = neighborsVector(g);
    int verticesNumber = num_vertices(g); 
    for(int i=0;i<verticesNumber;i++)
    {
        for(int j=0; j<verticesNumber;j++)
        {
            if(i !=j)
            {
                if(neighborsTest(i,j, tab) == false)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

/**
* Construction of a vector containing all the neighbors of each vertex
*/
std::vector<std::vector<int> > neighborsVector(phoeg::Graph& g)
{
    using namespace boost;

    std::vector<std::vector<int> > neighborsTab(num_vertices(g));

    typedef typename graph_traits<phoeg::Graph>::edge_iterator eiter;
    std::pair<eiter, eiter> ep;

    for(ep = edges(g); ep.first != ep.second; ++ep.first)//Recovering each edge of the graph (u,v)
    {
        int u = source(*ep.first,g), v = target(*ep.first,g);
        neighborsTab[u].push_back(v);
        neighborsTab[v].push_back(u);
    }

    return neighborsTab;
}

#endif
