#include <cstdio>
#include <string>
#include "nauty.h"

std::string cannonForm(std::string sig, int n, int m)
{
    n = 3;
    m = 2;
    graph array[n*m], canon[n*m];
    EMPTYGRAPH(array, m, n);
    ADDONEEDGE(array, 0, 1, m);
    ADDONEEDGE(array, 1, 2, m);
    set *row;
    for (long i = 0; i < n; i++)
    {
        row = GRAPHROW(array, i, m);
        for (long j = 0; j < n; j++)
        {
            if (ISELEMENT(row, j))
            {
                printf("%d ",1);
            }
            else
            {
                printf("%d ",0);
            }
        }
        printf("\n");
    }
    return "";
}
