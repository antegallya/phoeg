#include "nautyInterface.hpp"
#include "graph.hpp"
#include "graph6.hpp"
#include "invariants.hpp"

using namespace std;
using namespace phoeg;

typedef pair<vertex_iter,vertex_iter> vertex_iter_p;

void printGraph(const Graph & g)
{
    int n = order(g);
    for (vertex_iter_p u = vertices(g); u.first != u.second; u.first++)
    {
        for (vertex_iter_p v = vertices(g); v.first != v.second; v.first++)
        {
            printf("%d ",(edge(*u.first,*v.first,g).second ? 1 : 0));
        }
        printf("\n");
    }
}

int main()
{
    Graph g = convertFromGraph6("DV{");
    printGraph(g);
    Graph r = cannonForm(g);
    printf("%s\n",convertToGraph6(r).c_str());
    printGraph(r);
}
