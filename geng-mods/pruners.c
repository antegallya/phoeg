#include "gtools.h"

/* Claw-free pruner. */
int clawfree(graph *g, int n, int maxn) {
    int i, j, k;
    const int m = 1;
    const int v = n - 1;
    set *sv = GRAPHROW(g, v, m);
    setword sw1, sw2;
    for (i = -1; (i = nextelement(sv,m,i)) >= 0;) {
        /* Look for a claw with v as the articulation vertex, and i as one of
         * the fingertips. */
        /* neighbors of v but not of i. */
        sw1 = *sv & ~(*(GRAPHROW(g, i, m)));
        for (j = i; (j = nextelement(&sw1,m,j)) >= 0;) {
            /* neighbors of v but of neither i nor j. */
            sw2 = sw1 & ~(*(GRAPHROW(g, j, m)));
            for (k = j; (k = nextelement(&sw2,m,k)) >= 0;) {
                return 1;
            }
        }
        /* Look for a claw with v as one of the fingertips and i as the
         * articulation vertex. */
        /* Neighbors of i but not of v. */
        sw1 = ~(*sv) & *(GRAPHROW(g, i, m)) & ~BITT[v];
        for (j = -1; (j = nextelement(&sw1,m,j)) >= 0;) {
            /* Neighbors of i but of neither v nor j. */
            sw2 = sw1 & ~(*(GRAPHROW(g, j, m)));
            for (k = j; (k = nextelement(&sw2,m,k)) >= 0;) {
                return 1;
            }
        }
    }
    return 0;
}
