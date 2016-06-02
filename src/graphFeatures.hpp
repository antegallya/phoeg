#include <iostream>
#include <cmath>
#include "graph.hpp"

using namespace std;
using namespace boost;
using namespace phoeg;

//Declaration of methods
vector<vector<int> > neighborsVector(Graph& g);
/**
* Provides features on the graph
*/
void describe(Graph& g, const char* name)
{
  cout << "Ensemble des sommets du graphe: ";
  print_vertices(g, name);
  cout << endl;

  cout << "Ensemble des arêtes du graphe: ";
  print_edges(g, name);
  cout << endl;

  cout << "Arètes incidentes: " << endl;
  print_graph(g, name);
  cout << endl;
  
  cout <<  " Nombre de sommets : " <<  num_vertices(g) << endl;

}


/**
* Check if two vertices are neighbors  => Not used (see neighborsTest())
*/
bool neighbors(Graph& g, int i, int j) 
{
   typedef typename graph_traits<Graph>::edge_iterator eiter;
   pair<eiter, eiter> ep;  
   
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
bool neighborsTest(int i, int j, vector<vector<int> > v) 
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
void printNeighbors(vector<vector<int> > tab)
{
    for(int i=0; i < (int) tab.size(); i++)
    {
        for(int j =0 ; j < (int) tab[i].size(); j++)
        {
            cout <<  tab[i][j] << endl;
        }
    }
}

/**
* Print all edges for a graph
*/
void printEdges(Graph& g)
{
    typedef typename graph_traits<Graph>::edge_iterator eiter;
    pair<eiter, eiter> ep;  
    
    for (ep = edges(g); ep.first != ep.second; ++ep.first)//Recovering each edge of the graph (u,v)
    {
       int u = source(*ep.first,g), v = target(*ep.first,g);
       cout <<  " ( " << u << " , " << v << ")"  << endl;       
    }
}

/**
* Return the edges number for a graph
*/
int edgesNumber(Graph& g)
{
    typedef typename graph_traits<Graph>::edge_iterator eiter;
    pair<eiter, eiter> ep;  
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
bool isCompleteGraph(Graph& g)
{
    vector<vector<int> > tab = neighborsVector(g);
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
vector<vector<int> > neighborsVector(Graph& g)
{
    vector<vector<int> > neighborsTab(num_vertices(g));
    
    typedef typename graph_traits<Graph>::edge_iterator eiter;
    pair<eiter, eiter> ep;  
   
    for(ep = edges(g); ep.first != ep.second; ++ep.first)//Recovering each edge of the graph (u,v)
    {
        int u = source(*ep.first,g), v = target(*ep.first,g);
        neighborsTab[u].push_back(v);
        neighborsTab[v].push_back(u);
    } 
    return neighborsTab;      
  
}
