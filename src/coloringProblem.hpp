#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include "graph.hpp"
#include "invariants.hpp"
#include <cmath>
#include <string>
#include <sys/time.h>
#include <fstream>

using namespace std;
using namespace boost;
using namespace phoeg;


//Declaration of methods
int chromaticNumberSat(Graph& g, int color);

bool solutionFound = false;
vector<vector<int> > tab;
bool firstIter=true;


/**
*Computes a vertex coloring for the vertices in the graph.
*The heuristic provides an approximation for the SAT method.
*/
int coloration(Graph& g)
{
    typedef property_map<Graph, vertex_index_t>::const_type vertex_index_map;
    std::vector<vertices_size_t> color_vec(num_vertices(g));
    iterator_property_map<vertices_size_t*, vertex_index_map> color(&color_vec.front(), get(vertex_index, g));

    //Compute the coloration
    vertices_size_t num_colors = sequential_vertex_coloring(g, color);

    return num_colors;
}

/**
* Coding of proposals (variables) for the SAT.
*/
inline int prop(int n, int s, int c)
{
    return (n * (n + 1) - (n - c + 1) * (n - c + 2)) / 2 + s - c;
}

/**
* Call the main method (SAT method) with a good approximation.
*/
int chromaticNumber(Graph& g)
{
    int approximation = coloration(g);
    return chromaticNumberSat(g, approximation);
}

/**
* Call the main method (SAT method) with a good approximation on graph6 format.
*/
int chromaticNumber(string graph6)
{
    Graph g = convertFromGraph6(graph6);
    return chromaticNumber(g);
}

/**
* Calculate the chromatic number of a graph with a SAT problem.
*/
int chromaticNumberSat(Graph& g, int color)
{
    int n = num_vertices(g);
    int varNumber = (n * (n + 1) - (n - color) * (n - color + 1)) / 2;

    Solver solver;
    vec<Lit> lits;

    //Creation of variables
    for(int i = 0 ;i < varNumber; i++)
    {
        solver.newVar();
    }

    // Constraint neighboring
    for(int i = 1; i <= n; i++) 
    {
        for(int j = 1; j <= n; j++) 
        {
            if (edge(i-1,j-1,g).second)
            {
                for(int c = 1 ; c <= std::min(std::min(i, j),color); c++)
                {
                    solver.addBinary(~Lit(prop(n, i, c)),
                                     ~Lit(prop(n, j, c)));
                }
            }
        }
    }

    // Constraint existence : One color by node
    for(int i = 1; i <= n; i++) 
    {
        lits.clear();
        for(int c = 1 ; c <= std::min(color, i); c++) 
        {
            lits.push(Lit(prop(n, i, c)));
        }
        solver.addClause(lits);
    }

    int result = solver.solve();//Solve the sat problem
    if(result == 1) //Satisfiable
    {
        solutionFound= true;
        if(color > 1)
           return chromaticNumberSat(g, color-1); //Recursive call with k-1
        else // Particular case where we return k=1 (k must be greater than 0)
        {
            firstIter = true;//Re-initiate for the next call
            return color;
        }
    }
    else//Insatisfiable
    {
        if(solutionFound)//Base case
        {
            firstIter = true;//Re-initiate for the next call
            return color + 1;//Return the chromatic number
        }
        else//Particular case where the SAT formula is not satisfiable and we don't have a solution
        {
            if(color < n)
                return chromaticNumberSat(g, color+1); //Recursive call with k+1
        }
        
    }
    return color;//Return value in case of problems
}



