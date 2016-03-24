#include <cstdio>
#include <string>
#include "nauty.h"

using namespace std;

int getStart(int n)
{
    if (n > 258047)
    {
        return 9;
    }
    else if (n > 62)
    {
        return 5;
    }
    else
    {
        return 1;
    }
}

void fillArray(const string & graph6, int n, int m, graph *array)
{
    int start = getStart(n);
    if (graph6.size() > 0)
    {
        long p = start;
        char v = graph6[p]-63;
        int l = 5;
        for (long i = 1; i < n; i++)
        {
            for (long j = 0; j < i; j++)
            {
                if (l < 0)
                {
                    l = 5;
                    p++;
                    v = graph6[p]-63;
                }
                //We get the value of the bit in position l
                if ((v % (1<<(l+1)))>>l)
                {
                    ADDONEEDGE(array, i, j, m);
                }
                l--;
            }
        }
    }
}

string makeSig(const string& graph6, int n, int m, graph* array)
{
    int start = getStart(n);
    string res = graph6.substr(0,start);
    int l = 0;
    char r = 0;
    set *row;
    //We skip the first vertex since we want the inferior
    //half of the adjacency matrix
    for (int i = 1; i < n ; ++i)
    {
        row = GRAPHROW(array, i, m);
        for (int j = 0; j < i; ++j)
        {
            if (l == 6)
            {
                res += (char) r+63;
                l = 0;
                r = 0;
            }
            //We add a bit if there is an edge
            bool isEdge = ISELEMENT(row, j);
            r = (r << 1) + (isEdge ? 1 : 0);
            l++;
        }
    }
    //We might have to add 0's at the end of the last byte
    //if it wasn't long enough
    if (l <= 6)
    {
        r = r << (6-l);
        res += (char) r+63;
    }
    return res;
}

std::string cannonForm(std::string sig, int n, int m)
{
    int lab[n], ptn[n], orbit[n];
    static DEFAULTOPTIONS_GRAPH(options);
    options.getcanon = 1;
    statsblk stats;
    graph array[n*m], canon[n*m];
    EMPTYGRAPH(array, m, n);
    fillArray(sig, n, m, array);
    densenauty(array, lab, ptn, orbit, &options, &stats, m, n, canon);
    return makeSig(sig, n, m, canon);
}
