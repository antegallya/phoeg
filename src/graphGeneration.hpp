#include "graph.hpp"
#include <iostream>

using namespace std;
using namespace boost;
using namespace phoeg;



/**
* Generate a random graph (enough dense)
*/
Graph generRandomGraph(int verticesNumber)
{
    Graph g(verticesNumber);
    int source;
    int target;
    int maximum = (verticesNumber*(verticesNumber-1))/2; //Maximum number of edges in a graph

    int iter = rand() % maximum;
    int k =0;
    while(k < iter)
    {
        k = k+1;
        source = rand() % verticesNumber; //Generates a number between 0 and the number of vertices -1
        target = rand() % verticesNumber;                        
        if(source != target)//Avoid loops
        {
            add_edge(source, target, g);
        }
    }
    return g;
}
/**
* Generate a complete graph
*/
Graph generCompleteGraph(int verticesNumber)
{
    Graph g(verticesNumber);
    for(int i=0;i<verticesNumber;i++)
    {
        for(int j=0; j<verticesNumber;j++)
        {
            if(i !=j)
            {
                add_edge(i, j, g);
            }  
        }                      
    }
    return g;
}
