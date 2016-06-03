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
vector<vector<int> > neighborsVector(Graph& g);
bool neighborsTest(int i, int j, vector<vector<int> > v);
int chromaticNumberSat(Graph& g, const char* name,  int color, string file);
bool isCompleteGraph(Graph& g);


bool solutionFound = false;
int nodesNumber;
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
int prop(int s, int c)
{ 
   return ((s-1)*nodesNumber + c)-1;
}

/**
* Call the main method (SAT method) with a good approximation.
*/
int chromaticNumber(Graph& g, const char* name, string file)
{
    int approximation = coloration(g);
    nodesNumber = num_vertices(g);
    return chromaticNumberSat(g, name, approximation, file);
}

/**
* Call the main method (SAT method) with a good approximation on graph6 format.
*/
int chromaticNumber(string graph6, const char* name, string file)
{
    Graph g = convertFromGraph6(graph6);
    return chromaticNumber(g, name, file);
}


/**
* Calculate the chromatic number of a graph with a SAT problem.
*/
int chromaticNumberSat(Graph& g, const char* name,  int color, string file)
{
	int varNumber = (nodesNumber-1)*nodesNumber+color;
	
    Solver solver;
    vec<Lit> lits;

    //Creation of variables
    for(int i = 0 ;i < varNumber; i++)
    {
        solver.newVar();
    }

    // Constraint neighboring
    for(int i = 1; i <= nodesNumber; i++) 
    {
        for(int j = 1; j <= nodesNumber; j++) 
        {
	        if (edge(i-1,j-1,g).second)
	        {
	            for(int c = 1 ; c <= color; c++) 
	            {
		            solver.addBinary(~Lit(prop(i,c)),~Lit(prop(j,c)));
	            }
            }
        }
    }

    // Constraint existence : One color by node
    for(int i = 1; i <= nodesNumber; i++) 
    {
        lits.clear();
	    for(int c = 1 ; c <= color; c++) 
        {
        	lits.push(Lit(prop(i,c)));
        }
        solver.addClause(lits);
    }

    int result = solver.solve();//Solve the sat problem
    if(result == 1) //Satisfiable
    {
        solutionFound= true;
        if(file.length() > 0)//Writing results in the file
        {
            if(firstIter == true)//File creation
            {
                if(file.substr(file.length()-4,file.length()-1).compare(".txt") != 0)//Add extension "txt"
                {
                    file = "Results/DisplaySolutions/" +  file + ".txt";
                }
                else
                {
                    file = "Results/DisplaySolutions/" +  file; 
                }
                firstIter = false;
            }

            ofstream resultFile(file.c_str(), ios::out);//Opening the file for writing

            if(resultFile.is_open())  
            {
                resultFile << "Displaying of the solution with the chromatic number = " << color <<  endl;
                resultFile << ""  <<  endl;

                for(int s = 1 ; s <= nodesNumber; s++) 
                {
                    for(int c = 1; c <= color; c++) 
                    {
                        if (solver.model[prop(s,c)] == l_True) 
                        {
                            if(name != NULL)
                            {
                                resultFile << "The vertice " << name[s-1] << " is colored with the color " << c <<  endl;
                            }
                            else
                            {
                                resultFile << "The vertice " << s-1 << " is colored with the color " << c <<  endl;
                            }   
                        }
                    }   
                }
                resultFile.close();//Close file
            }
            else
                cerr << "Can not open file for writing ! " << endl; 
        }
        if(color > 1)
	       return chromaticNumberSat(g, name, color-1, file); //Recursive call with k-1
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
    		if(color < nodesNumber)
    			return chromaticNumberSat(g, name, color+1, file); //Recursive call with k+1
    	}
        
    }
    return color;//Return value in case of problems
}



