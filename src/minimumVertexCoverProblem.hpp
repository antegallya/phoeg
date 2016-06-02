#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include "graph.hpp"
#include "invariants.hpp"
#include <cmath>

#include <list>
#include <iostream>
#include <unistd.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

using namespace std;
using namespace boost;
using namespace phoeg;



int **adjencyMatrix;//Adjency matrix for the graph
int **arrayPointer;//Pointer's array used to create clause
int *solution;//Array containing the final solution
int verticesNumber;
bool solFound = false;
bool first = true;

//Declaration of methods
int approxMinVertexCover(Graph& g);


/*
* Creating arrays and values initialization
*/
void initialisation(Graph& g)
{	

	if(!solFound)
	{
		//Allocate memory for the array contaning the final solution
		solution=(int*)malloc(verticesNumber*sizeof(int));
	}
	int valeur = 0;

	// Allocate memory for pointer to each row
	adjencyMatrix = (int**) malloc((verticesNumber) * sizeof(int*));
	arrayPointer = (int**) malloc((verticesNumber) * sizeof(int*));

        
	// Allocate memory for each row
	for(int i = 0; i < verticesNumber; i++)
    {	
    	adjencyMatrix[i] = (int*) malloc((verticesNumber) * sizeof(int));	  
		arrayPointer[i] = (int*) malloc((verticesNumber) * sizeof(int));	  
	}
	
    vector<vector<int> > tab = neighborsVector(g);//Creating an array containing the neighbors for each vertice

    //creation of the adjacency matrix of the graph and values initialisation of pointers (array containing codages of proposals)
	for(int i = 0; i < verticesNumber; i++) 
    {
        for(int j = 0; j < verticesNumber; j++) 
        {
	        if (neighborsTest(i,j,tab))
	        {
	        	adjencyMatrix[i][j]=1;
				adjencyMatrix[j][i]=1;
	        }
	        else
	        {
	           	adjencyMatrix[i][j] = 0;
	        }
	        arrayPointer[j][i] = valeur;//Add codage of proposals in the array
	        valeur++;
	    }
	}
}

/**
* Comparison method between two elements
*/
int compare (const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}


/**
* Compute the Minimum Vertex Cover with SAT
*/
void mvcSat(Graph& g, int k)
{ 
	int varNumber = verticesNumber*k;

	Solver solver;
    vec<Lit> lits;

    //Creation of proposals
    for(int i = 0 ; i < varNumber; i++)
    {
        solver.newVar();
    }


    //Constraint : Each vertice can belong in the cover
	for(int j=0; j<k; j++)
	{ 	
		lits.clear();
		for(int i=0; i<verticesNumber; i++)
		{
			lits.push(Lit(arrayPointer[i][j]));
		}
		solver.addClause(lits);
	}

	int p;
	//Constraint : Each vertice belonging to the cover must be different
	for(int m=0; m<verticesNumber; m++)
	{
		for(int i=1; i<k; i++)
		{
			p=i;
			for(int q=p+1; q<=k; q++)//Create binary clause
			{	
				lits.clear();
				lits.push(~Lit((arrayPointer[m][p-1])));
				lits.push(~Lit((arrayPointer[m][q-1])));
				solver.addClause(lits);
			}	
		}
	}
	//Constraint : Each set must be covered by only one vertice
	for(int m=1; m<=k; m++)
	{
		for(int i=1;i<verticesNumber;i++)
		{
			p=i;
			for(int q=p+1; q<=verticesNumber; q++)//Create binary clause
			{	
				lits.clear();
				lits.push(~Lit((arrayPointer[p-1][m-1])));
				lits.push(~Lit((arrayPointer[q-1][m-1])));
				solver.addClause(lits);											
			}
        }
	}
        
	//Constraint : Each edge must be covered
	for(int i=0;i< verticesNumber;i++)
	{ 	
		for(int j=i; j<verticesNumber; j++)
		{
			if(adjencyMatrix[i][j]==1)
			{
				lits.clear();
				for(int l=0; l<k; l++)		
				{
					lits.push(Lit(arrayPointer[i][l]));
					lits.push(Lit(arrayPointer[j][l]));	
				}
				solver.addClause(lits);
			}
		}		
	}
	
	int result = solver.solve();//Solve the sat problem

    if(result == 1) //Satisfiable
	{
		solFound=true;

		for(int i = 0; i < verticesNumber; i++) 
	    {
	    	solution[i]=1000;//Default value in array "solution"
	    }

		int indice = 0;
		for(int j=0; j < varNumber; j++) 
		{
	    	if(solver.model[j] == l_True) 
	    	{               
                for(int row=0; row < verticesNumber; row++)
				{
					for(int col=0; col < verticesNumber;col++)
					{
						if(arrayPointer[row][col]==j)
						{
							solution[indice]=row;
							indice++;
						}
					}
				}							
						
	    	}
		}
		if(k > 0)//Check if there is a best solution
		{
			initialisation(g);//Re-initiate of matrix and array
			mvcSat(g, k-1);//Recursive call with k-1
		}
    }     
}

/**
* return true if s is in the array, false otherwise
*/
bool inTab(int array[], int s, int length)
{
	for(int i=0; i<length; i++)
	{
		if(array[i] == s)
		{
			return true;
			break;
		}
	}
	return false;
}

/**
* Calculate the minimum vertex cover of a graph with a SAT problem
*/
int minimumVertexCoverSAT(Graph& g, const char* name, int k, bool boolean, string file)
{

	initialisation(g);//Matrix initialisation 

    //Resolution of the problem with the SAT solver (k fixed)
    mvcSat(g,k);


    qsort(solution,verticesNumber,sizeof(int),compare);//Sort the vertices

    //Displaying solution
	int result=0;
	if(file.length() > 0)//Writing results in the file
    {
    	if(first == true)//File creation
    	{
    		if(file.substr(file.length()-4,file.length()-1).compare(".txt") != 0)//Add extension "txt"
	        {
	            file = "Results/DisplaySolutions/" +  file + ".txt";
	        }
	        else
	        {
	       		if(!solutionFound) 
	   				file = "Results/DisplaySolutions/" +  file; 
	       	}
	       	first = false;
    	}
       	ofstream resultFile(file.c_str(), ios::out);//Opening the file for writing

        if(resultFile.is_open())  
        {
        	if(boolean == false)//Solution of Minimum Vertex Cover
			{
	        	resultFile << "Displaying of the solution with the Minimum Vertex Cover for the graph " << convertToGraph6(g) << "  (in format 6)"  <<  endl;
	            resultFile << ""  <<  endl;
	            for(int j=0; j<verticesNumber; j++)
				{ 
					if(solution[j] != 1000)
					{
						if(name != NULL)
				        {
				        	resultFile << "The vertice " << name[solution[j]] << " belongs to the Minimum Vertex Cover " <<  endl;
				        }
				        else
				        {
				        	resultFile << "The vertice " << solution[j] << " belongs to the Minimum Vertex Cover " <<  endl;
				        }
						result++;
					}
				}
				resultFile.close();//Close file
			}
			else//Solution of Maximum Independent Set
			{
				resultFile << "Displaying of the solution with the Maximum Independent Set  for the graph " << convertToGraph6(g) << "  (in format 6)" <<  endl;
	            resultFile << ""  <<  endl;
				for(int j=0; j<verticesNumber; j++)
				{
					if(inTab(solution,j,verticesNumber)==false)
					{
						if(name != NULL)
				        {
				        	resultFile << "The vertice " << name[j] << " belongs to the Maximum Independent Set" <<  endl;
				        }
				        else
				        {
				        	resultFile << "The vertice " << j << " belongs to the Maximum Independent Set " <<  endl;
				        }
					}
					else
					{
				        result++;
					}
				}
				resultFile.close();//Close file
			}
		}
		else
        	cerr << " Can not open file for writing ! " << endl; 
    }
    else //No wrinting in a file
    {
    	if(boolean == false)
    	{
    		for(int j=0; j<verticesNumber; j++)
			{ 
				if(solution[j] != 1000)
				{
					result++;
				}
			}
		}
		else
		{
			for(int j=0; j<verticesNumber; j++)
			{
				if(inTab(solution,j,verticesNumber)==true)
				{
					result++;
				}
			}
    	}
    }
    first = true;//Re-initiate for the next call
	return result;//The solution of the problem
}

/**
* Call the main method (SAT method) with a good approximation.
*/
int minimumVertexCover(Graph& g, const char* name, bool boolean, string file)
{
	verticesNumber = num_vertices(g);
    int approximation = approxMinVertexCover(g);
    return minimumVertexCoverSAT(g, name, approximation, boolean, file);
}
/**
* Call the main method (SAT method) with a good approximation on graph6 format.
*/
int minimumVertexCover(string graph6, const char* name, bool boolean, string file)
{

    Graph g = convertFromGraph6(graph6);
    return minimumVertexCover(g, name, boolean, file);
}


/**
*Computes a Vertex Cover by minimizing his size
*The heuristic provides an approximation for the SAT method
* (Source = http://www.geeksforgeeks.org/vertex-cover-problem-set-1-introduction-approximate-algorithm-2/)
*/
int approxMinVertexCover(Graph& g)
{
	list<int> *adj;
	adj = new list<int>[verticesNumber];

	vector<vector<int> > tab;

	tab = neighborsVector(g);//Creating an array containing the neighbors for each vertice

	typedef typename graph_traits<Graph>::edge_iterator eiter;
    pair<eiter, eiter> ep;  
    
    for (ep = edges(g); ep.first != ep.second; ++ep.first)
    {
       	int u = source(*ep.first,g), v = target(*ep.first,g);
       	adj[u].push_back(v); // Add w to v’s list.
		adj[v].push_back(u); // Since the graph is undirected         
    }


   	// Initialize all vertices as not visited.
	bool visited[verticesNumber];
	for (int i=0; i<verticesNumber; i++)
		visited[i] = false;

	list<int>::iterator i;

	// Consider all edges one by one
	for (int u=0; u<verticesNumber; u++)
	{
		// An edge is only picked when both visited[u] and visited[v] are false
		if (visited[u] == false)
		{
			// Go through all adjacents of u and pick the first not
			// yet visited vertex (We are basically picking an edge
			// (u, v) from remaining edges.
			for (i= adj[u].begin(); i != adj[u].end(); ++i)
			{
				int v = *i;
				if (visited[v] == false)
				{
					// Add the vertices (u, v) to the result set.
					// We make the vertex u and v visited so that
					// all edges from/to them would be ignored
					visited[v] = true;
					visited[u] = true;
					break;
				}
			}
		}
	}
	int result = 0;
	// Compute the vertex cover
	for (int i=0; i<verticesNumber; i++)
		if (visited[i])
		{
			result++;
		}
    return result;
}

