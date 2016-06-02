#include <iostream>
#include "graph.hpp"


using namespace std;
using namespace boost;
using namespace phoeg;

//Declaration of methods
int notMarquedVertices(vector<int> vector);
bool neighbors(Graph& g, int i, int j);
vector<vector<int> > neighborsVector(Graph& g);
bool neighborsTest(int i, int j, vector<vector<int> > v);
int minimumVertexCover(Graph& g, const char* name, bool boolean, string file);


/**
* Calculates the size of a maximum independent set using the 
* size of minimum vertex cover for a graph
*/
int maximumIndependentSet(Graph& g, const char* name, string file)
{

    int complementary =  minimumVertexCover(g, name, true, file);
    return num_vertices(g)-complementary;
}

/**
* Calculates the size of a maximum independent set using the 
* size of minimum vertex cover for a graph in format graph6
*/
int maximumIndependentSet(string graph6, const char* name, string file)
{
    Graph g = convertFromGraph6(graph6);
    return maximumIndependentSet(g, name, file);
}



/**
* Computes the length of a independent set in the graph (not necessarily the greatest)
* Provides a approximation
*/
int findIndependentSet(Graph& g)
{
    int verticesNumber = num_vertices(g);
    vector<int> marqueVertices(verticesNumber);

    bool allIsMarqued = false;
    vector<int> maximalSetIndependent;
    vector<vector<int> > tab = neighborsVector(g);

    int i;
    while(allIsMarqued == false)
    {
        i=notMarquedVertices(marqueVertices);
        if(i==-1)
        {
            allIsMarqued = true;
        }
        else
        {           
            marqueVertices[i]=1;
            maximalSetIndependent.push_back(i);
            for(int j=0; j<verticesNumber; j++)
            {
                if(neighborsTest(j, i, tab) == 1)
                {
                    marqueVertices[j]=1;//Marqued vertice's neighbors
                }
            }
        }
    }   
    return maximalSetIndependent.size();//Return a approximation of the best size
}


/**
* Returns the position of an unmarked vertice
* Source = http://www.sanfoundry.com/cpp-program-find-independent-sets-graph-by-graph-coloring/
*/
int notMarquedVertices(vector<int> vector)
{
    int j=0;
    while(j < (int) vector.size())
    {
        if(vector[j] == 0)
        {
            return j;
        }
        else
        {
            j = j+1;
        }
    }
    //End value
    return -1;
}

 /**
 * Method used in maxIndependentSet() to find a maximum independent set  (not a approximation)
 * Source = http://www.sanfoundry.com/cpp-program-find-independent-sets-graph-by-graph-coloring/
 */ 
bool removable(vector<int> neighbor, vector<int> cover)
{
    bool check = true;

    for (int i=0; i<(int) neighbor.size(); i++)
    if (cover[neighbor[i]] == 0)
    {
        check = false;
        break;
    }
    return check;
}
 /**
 * Method used in maxIndependentSet() to find a maximum independent set  (not a approximation)
 * Source = http://www.sanfoundry.com/cpp-program-find-independent-sets-graph-by-graph-coloring/
 */ 
int max_removable(vector<vector<int> > neighbors, vector<int> cover)
{
    int r = -1;
    int max = -1;

    for (int i=0; i<(int) cover.size(); i++)
    {
        if (cover[i] == 1 && removable(neighbors[i], cover) == true)
        {
            vector<int> temp_cover = cover;
            temp_cover[i] = 0;
            int sum = 0;

            for (int j=0; j<(int) temp_cover.size(); j++)
                if (temp_cover[j] == 1 && removable(neighbors[j], temp_cover)== true)
                {
                    sum++;
                }

            if (sum > max)
            {
                max = sum;
                r = i;
            }
        }
    }
    return r;
}
  /**
 * Procedure used in maxIndependentSet() to find a maximum independent set  (not a approximation)
 * Source = http://www.sanfoundry.com/cpp-program-find-independent-sets-graph-by-graph-coloring/
 */
vector<int> procedure_1(vector<vector<int> > neighbors, vector<int> cover)
{
    vector<int> temp_cover = cover;
    int r = 0;
    while (r != -1)
    {
        r = max_removable(neighbors, temp_cover);
        if (r != -1)
        {
            temp_cover[r] = 0;
        }
    }
    return temp_cover;
}
 /**
 * Procedure used in maxIndependentSet() to find a maximum independent set  (not a approximation)
 * Source = http://www.sanfoundry.com/cpp-program-find-independent-sets-graph-by-graph-coloring/
 */
vector<int> procedure_2(vector<vector<int> > neighbors, vector<int> cover, int k)
{
    int count = 0;
    vector<int> temp_cover = cover;

    for (int i=0; i<(int) temp_cover.size(); i++)
    {
        if (temp_cover[i] == 1)
        {
            int sum = 0, index;
            for (int j = 0; j < (int) neighbors[i].size(); j++)
                if (temp_cover[neighbors[i][j]] == 0)
                {
                    index = j;
                    sum++;
                }
            if (sum == 1 && cover[neighbors[i][index]] == 0)
            {
                temp_cover[neighbors[i][index]] = 1;
                temp_cover[i] = 0;
                temp_cover = procedure_1(neighbors, temp_cover);
                count++;
            }
            if (count > k)
                break;
        }
    }
    return temp_cover;
}
/**
* Return the cover size
* Source = http://www.sanfoundry.com/cpp-program-find-independent-sets-graph-by-graph-coloring/
*/
int cover_size(vector<int> cover)
{
    int count = 0;
    for (int i=0; i<(int) cover.size(); i++)
        if (cover[i] == 1)
            count++;
    return count;
}



/**
* Calculate the size of a maximum independent set for a graph
* This method was used to test the result of SAT method in the past => Not used
* Source = http://www.sanfoundry.com/cpp-program-find-independent-sets-graph-by-graph-coloring/ 
*/
int maxIndependentSet(Graph& g, int K)
{
    int n; 
    n = num_vertices(g);
    vector<vector<int> > graph;

    //creation of the adjacency matrix of the graph
    for (int i=0; i<n; i++)
    {
        vector<int> row;
        for (int j = 0; j < n; j++)
        {
            if(neighbors(g,i,j)==1)
            {
                row.push_back(1);
            }
            else
            {
                row.push_back(0);
            } 
        }
        graph.push_back(row);
    }

    vector<vector<int> > neighborsTab = neighborsVector(g);//Creating an array containing the neighbors for each vertice

    int k = n-K;
    vector<int> solution;//A array containing the final solution
    
    //Find Independent Sets
    bool found = false;
    int min = n+1;
    vector<vector<int> > covers;
    vector<int> allcover;

    for (int i=0;i< (int) graph.size(); i++)
    {
        allcover.push_back(1);
    }
    for (int i=0; i<(int) allcover.size(); i++)
    {
        if (found)
        {
            //We found a maximum independent set for the graph
            break;
        }

        vector<int> cover = allcover;
        cover[i] = 0;
        cover = procedure_1(neighborsTab, cover);
        int s = cover_size(cover);
        if (s < min)
        {
            min = s;
        }

        if (s <= k)
        {
            for (int j=0; j<(int) cover.size(); j++)
            {
                if (cover[j] == 0)
                {
                    solution.push_back(j);
                }
            }
            covers.push_back(cover);
            found = true;
            break;
        }

        for (int j=0; j<n-k; j++)
        {
            cover = procedure_2(neighborsTab, cover, j);
        }

        s = cover_size(cover);

        if (s < min)
        {
            min = s;
        }

        for (int j=0; j<(int) cover.size(); j++)
        {
            if (cover[j] == 0)
            {
                solution.push_back(j);
            }
        }

        covers.push_back(cover);

        if (s <= k)
        {
            found = true;
            break;
        }
    }
    //Pairwise Intersections
    for(int p=0; p<(int) covers.size(); p++)
    {
        if (found)
        {
            break;
        }
        for (int q=p+1; q<(int) covers.size(); q++)
        {
            if (found)
            {
                break;
            }

            vector<int> cover = allcover;

            for (int r=0; r<(int) cover.size(); r++)
            {
                if (covers[p][r] == 0 && covers[q][r] == 0)
                {
                    cover[r] = 0;
                }
            }
            cover = procedure_1(neighborsTab, cover);
            int s = cover_size(cover);

            if (s < min)
            {
                min = s;
            }

            if (s <= k)
            {
                for (int j=0; j<(int) cover.size(); j++)
                {
                    if (cover[j] == 0)
                    {
                        solution.push_back(j);
                    }
                }
                found = true;
                break;
            }

            for (int j=0; j<k; j++)
            {
                cover = procedure_2(neighborsTab, cover, j);
            }
            s = cover_size(cover);

            if (s < min)
            {
                min = s;
            }

            for (int j=0; j<(int) cover.size(); j++)
            {
                if (cover[j] == 0)
                {
                    solution.push_back(j);
                }
            }

            if (s <= k)
            {
                found = true;
                break;
            }
        }
    }
    if (found)
    {
        return K;//Return the solution
    }
    else
    {
        return maxIndependentSet(g , K-1);//Recursive call with k-1
    }
}


