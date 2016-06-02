#include <iostream>
#include "Minisat/Solver.hpp"
#include "SAT.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"
#include <cmath>
#include <sys/time.h>
#include <fstream>
#include <iomanip>
#include "coloringProblem.hpp"
#include "maximumIndependentSetProblem.hpp"
#include "graphFeatures.hpp"
#include "graphGeneration.hpp"
#include "minimumVertexCoverProblem.hpp"

using namespace std;
using namespace boost;
using namespace phoeg;

/**
* Test Method
*/
int main(int argc, char* argv[])
{
    //Creation of graph (Example of report)
    Graph g(4);
    add_edge(0, 2, g);
    add_edge(0, 3, g);
    add_edge(2, 3, g);
    add_edge(1, 3, g);

    const char* name = "1234";
   
    //Response Phoeg
    int mvcREF = minVertexCover(g);
    int chromaticREF = chromaticNumber(g);
    int misREF = maxIndependentSet(g);

    //Response SAT without storing the answer
    int chromaticSAT = chromaticNumber(g,name, "");//finalChromatic
    int mvcSAT = minimumVertexCover(g, name, false, "");//finalMVC
    int misSAT = maximumIndependentSet(g, name, "");//finalMIS

    //Response SAT with storing the answer
    chromaticSAT = chromaticNumber(g,name, "finalChromatic");
    mvcSAT = minimumVertexCover(g, name, false, "finalMVC");
    misSAT = maximumIndependentSet(g, name, "finalMIS");

    cout << "[CHROMATIC] Phoeg  : " << chromaticREF << "  <------------> SAT : " <<  chromaticSAT <<"\n";
    cout << "[MVC] Phoeg : " << mvcREF << " <-------------> SAT : " <<  mvcSAT <<"\n";
    cout << "[MIS] Phoeg : " << misREF << " <-------------> SAT : " <<  misSAT <<"\n";



    /*int chromaticSAT;
    int chromaticREF;
    int mvcSAT;
    int mvcREF;
    int misSAT;
    int misREF;
    int i =1;

    
    //Comparison between the SAT method and the phoeg method for graphs contained in a file
    ifstream fichierR("GraphsGenerating/3.txt");
    if(fichierR)
    {
        ofstream fichierE("Results/3.txt", ios::out | ios::trunc);
        if(fichierE)  
        {   
            //Creation of the header
            fichierE << "Graph6 format     "  << " Nodes number     " << " Edges Number     " << " Chromatic Number     " <<" Maximum Independent Set     "<< " Minimum Vertex Cover     "  <<  endl;
            fichierE << setw(70)  <<"(Phoeg - SAT) " << setw(25) << "(Phoeg - SAT) " << setw(30) << "(Phoeg - SAT) "  <<  endl;
            fichierE <<""<< endl;

            string ligne;
            while(getline(fichierR, ligne))
            {
            	//Convert the graph in file
                Graph g = convertFromGraph6(ligne);

                //Phoeg Response
                mvcREF = minVertexCover(g);
                chromaticREF = chromaticNumber(g);
                misREF = maxIndependentSet(g);

				//SAT Response
                mvcSAT = minimumVertexCover(g, NULL, false, "");
				chromaticSAT = chromaticNumber(g,NULL, "");
                misSAT = maximumIndependentSet(g, NULL, "");

                if(chromaticREF != chromaticSAT || mvcREF != mvcSAT || misSAT != misREF)
                	cout <<"[ERROR] Not the same value ! \n";

                fichierE << setw(5) <<  ligne << setw(20)  << num_vertices(g) << setw(20) << edgesNumber(g) << setw(18) << chromaticREF  << " - " << chromaticSAT  << setw(22) << misREF << " - " << misSAT << setw(20) << mvcREF << " - " << mvcSAT  <<  endl;
                i++; 
                fichierE << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";    
            }
            
            //Close files
            fichierR.close();  
            fichierE.close();
            cout << "[SUCCES] All graphs were calculated\n";
        }
        else  
            cerr << " Can not open file for writing ! " << endl;      
   }
   else
   {
      cout << " Can not open file for reading ! " << endl;
   }*/

    return 0;
}







