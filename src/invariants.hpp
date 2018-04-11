#ifndef INVARIANTS_CPP
#define INVARIANTS_CPP

#include "graph.hpp"
#include "transformations.hpp"

#include <boost/functional/hash.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <deque>

#define INF std::numeric_limits<long>::max()

namespace phoeg
{
    typedef std::vector<std::vector<long> > dMatrix;

    /**
     * Return the order of the graph g.
     * i.e., the number of nodes of g.
     */
    template<class Graph>
    long order(const Graph & g)
    {
        return num_vertices(g);
    }

  namespace detail {

    template <class Graph>
    bool is_locally_proper_color(const Graph & g,
                                 boost::shared_array<int>& col,
                                 const int & v, const int c, const int & nv)
    {
      int i;
      for (i = 0; i < nv; i++)
        if (edge(i, v, g).second && col[i] == c)
          return false;

      return true;
    }

    template <class Graph>
    bool col_rec(const Graph & g, int k,
                 boost::shared_array<int>& col,
                 int v = 0, bool robust = false);

    template <class Graph>
    bool is_locally_recolorable(const Graph & g, const int & k,
                                boost::shared_array<int>& col, const int & v)
    {
      bool found = false;
      int vc = col[v];
      int c;

      /* Try each color reassignation for v. */
      for (c = 1; c <= k && !found; c++) {
        /* Skip the original color since we want a strict recoloring. */
        if (c == vc)
          continue;

        col[v] = c;

        /* Try to recolor the neighbors of v. First reset their colors. */
        int i;
        for (i = 0; i < order(g); i++)
          if (edge(i, v, g).second)
            col[i] = 0;

        /* Then, reassign colors. */
        found = col_rec(g, k, col);
      }

      return found;
    }

    template <class Graph>
    bool is_robust_coloring(const Graph & g, const int & k,
                            const boost::shared_array<int>& col)
    {
      bool ok = true;
      boost::shared_array<int> tcol(new int[order(g)]);

      /* The coloring is robust if each vertex is locally recolorable. */
      for (int v = 0; v < order(g) && ok; v++) {
        std::copy(col.get(), col.get() + order(g), tcol.get());
        if (!is_locally_recolorable(g, k, tcol, v))
          ok = false;
      }

      return ok;
    }

    template <class Graph>
    bool col_rec(const Graph & g, int k,
                 boost::shared_array<int>& col, int v, bool robust)
    {
      if (v == order(g)) {
        return !robust || is_robust_coloring(g, k, col);
      }
      else if (col[v] != 0) {
        /* The color of that node is forced.
           Check its properness and continue. */
        return is_locally_proper_color(g, col, v, col[v], v) &&
          col_rec(g, k, col, v + 1, robust);
      }
      else {
        /* Induction hypothesis: v must be colored and all previous nodes are
           well-colored. */
        bool found = false;
        int c, i;
        for (c = 1; c <= k && !found; c++) {
          /* Can v be colored with c ? */
          if (is_locally_proper_color(g, col, v, c, v)) {
            /* If yes, continue this coloration. */
            col[v] = c;
            found = col_rec(g, k, col, v + 1, robust);
          }
        }

        /* Reset the color if it's not a good one. */
        if (!found)
          col[v] = 0;

        return found;
      }
    }

    template <class Graph>
    bool is_k_colorable(const Graph & g, const int & k, bool robust = false)
    {
      boost::shared_array<int> col(new int[order(g)]);
      int i;
      bool ok;

      if (order(g) > 0)
        col[0] = 1;

      for (i = 1; i < order(g); i++)
        col[i] = 0;

      ok = col_rec(g, k, col, 0, robust);

      return ok;
    }

    /* This function returns the number of colors used
       in a sequential vertex coloring */
    template <class Graph>
    int seq_colors(const Graph & g)
    {
      int n = order(g);
      boost::shared_array<int> col(new int[n]);
      int i, j;
      int nc = 0;
      int this_color;
      bool ok;

      for (i = 0; i < n; i++)
        col[i] = 0;

      for (i = 0; i < n; i++) {
        this_color = 0;
        ok = false;
        while(!ok) {
          ok = true;
          this_color++;
          for (j = 0; j < n && ok; j++) {
            if (col[j] == this_color && edge(i, j, g).second)
              ok = false;
          }
        }
        col[i] = this_color;
        if (nc < this_color)
          nc = this_color;
      }

      return nc;
    }
  } // namespace detail

    /**
     * Return the size of the graph g.
     * i.e., the number of edges of g.
     */
    template<class Graph>
    long numEdges(const Graph & g)
    {
        return num_edges(g);
    }

    /**
     * Return the distance matrix computed using
     * Floyd-Warshall algorithm.
     */
    template<class Graph>
    dMatrix floydWarshall(const Graph & g)
    {
        long n = order(g);
        dMatrix matrix;
        for (long i = 0; i < n; ++i)
        {
            std::vector<long> line;
            for (long j = 0; j < n; ++j)
            {
                if (i == j)
                {
                    line.push_back(0);
                }
                else if (edge(i,j,g).second)
                {
                    line.push_back(1);
                }
                else
                {
                    line.push_back(INF);
                }
            }
            matrix.push_back(line);
        }
        for (long k = 0; k < n; ++k)
        {
            for (long i = 0; i < n; ++i)
            {
                for (long j = 0; j < n; ++j)
                {
                    //If both distances are not infinite
                    if (matrix[i][k] != INF && matrix[k][j] != INF)
                    {
                        //If d(i,j) is infinite or distance bigger than d(i,k)+d(k,j)
                        if (matrix[i][j] == INF || matrix[i][j] > matrix[i][k] + matrix[k][j])
                        {
                            matrix[i][j] = matrix[i][k] + matrix[k][j];
                        }
                    }
                }
            }
        }
        return matrix;
    }

    /**
     * Computes the distance matrix of graph g
     */
    template <class Graph>
    dMatrix distanceMatrix(const Graph & g)
    {
        return floydWarshall(g);
    }

    /**
     * Return the average distance between all pairs of vertices in g.
     */
    template <class Graph>
    long averageDistance(const Graph & g)
    {
        int i, j;
        int n = order(g);
        int total_dist = 0;
        dMatrix dm = floydWarshall(g);

        for(i = 0; i < n; i++) {
            for (j = i + 1; j < n; j++) {
                total_dist += dm[i][j];
            }
        }

        return total_dist * 2 / (n * (n - 1));
    }

    /**
     * Returns the diameter of the graph g.
     * i.e., the maximal distance between two nodes.
     * If the graph is not connected, returns INF
     */
    template <class Graph>
    long diameter(const Graph & g)
    {
        dMatrix dist = distanceMatrix(g);
        long n = order(g);
        long max = -1;
        for (long i = 0; i < n; ++i)
        {
            //We need to consider the graph with n = 1
            //thus, j = i
            for (long j = i; j < n; ++j)
            {
                if (max < dist[i][j])
                {
                    max = dist[i][j];
                }
            }
        }
        return max;
    }

    /**
     * Returns the radius of the graph g.
     * i.e., the minimum of the largest distances
     * If the graph is not connected, returns INF
     */
    template <class Graph>
    long radius(const Graph & g)
    {
        dMatrix mat = distanceMatrix(g);
        long n = order(g);
        long rad = INF;
        for (long i = 0; i < n; ++i)
        {
            long max = -1;
            for (long j = 0; j < n; ++j)
            {
                if (mat[i][j] > max)
                {
                    max = mat[i][j];
                }
            }
            if (max < rad)
            {
                rad = max;
            }
        }
        return rad;
    }

    /**
     * Returns the list of degrees of the nodes
     * sorted in increasing order
     */
    template<class Graph>
    std::vector<long> listDegrees(const Graph & g)
    {
        std::vector<long> list(order(g), 0);
        typedef typename boost::graph_traits<Graph>::edge_iterator eiter;
        std::pair<eiter, eiter> ep;
        for (ep = edges(g); ep.first != ep.second; ++ep.first)
        {
            int u = source(*ep.first,g), v = target(*ep.first,g);
            list[u]++;
            list[v]++;
        }
        make_heap(list.begin(), list.end());
        sort_heap(list.begin(), list.end());
        return list;
    }

    /**
     * Returns the maximal degree of the graph g.
     * i.e., the maximal number of neighbors in the graph.
     */
    template<class Graph>
    long maxDegree(const Graph & g)
    {
        std::vector<long> degrees = listDegrees(g);
        if (degrees.size() > 0)
        {
            return degrees[degrees.size()-1];
        }
        else
        {
            return 0;
        }
    }

    /**
     * Returns the minimal degree of the graph g.
     * i.e., the minimal number of neighbors in the graph.
     */
    template<class Graph>
    long minDegree(const Graph & g)
    {
        std::vector<long> degrees = listDegrees(g);
        if (degrees.size() > 0)
        {
            return degrees[0];
        }
        else
        {
            return 0;
        }
    }

    /**
     * Tests weither the graph g is connected.
     */
    template<class Graph>
    bool isConnected(const Graph & g)
    {
        std::vector<int> comps(order(g));
        long num = connected_components(g, &comps[0]);
        return num < 2;
    }

    /**
     * Tests weither the graph g is a tree.
     * i.e., connected and n-1 edges.
     */
    template<class Graph>
    bool isTree(const Graph & g)
    {
        return isConnected(g) && (numEdges(g) == order(g)-1);
    }

    /**
     * Computes the number of non-equivalent
     * colorings of the graph g.
     */
    template<class Graph>
    long numCol(const Graph & g)
    {
        long n = order(g);
        if (n == 1 || minDegree(g) == n-1)
        {
            return 1;
        }
        typedef std::pair<vertex_iter, vertex_iter> p_vertex_iter;
        vertex u,v;
        bool nonEdgeFound = false;
        for (p_vertex_iter vp = vertices(g); vp.first != vp.second && !nonEdgeFound; ++vp.first)
        {
            for (vertex_iter w = vp.first+1; w != vp.second && !nonEdgeFound; ++w)
            {
                if (!edge(*vp.first, *w, g).second)
                {
                    u = *vp.first;
                    v = *w;
                    nonEdgeFound = true;
                }
            }
        }
        Graph g1 = contract(u, v, g);
        long num = numCol(g1);
        Graph g2(g);
        add_edge(u,v,g2);
        num += numCol(g2);
        return num;
    }

    /**
     * Computes the number of non-equivalent
     * k-colorings of the graph g.
     * FIXME: Could this be factored with numCol ?
     */
    template<class Graph>
    long numColK(const Graph & g, int k)
    {
        long n = order(g);
        long m = numEdges(g);
        if (k == n && m == 0)
        {
            return 1;
        }
        if (minDegree(g) == n-1)
        {
            if (k == n)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        typedef std::pair<vertex_iter, vertex_iter> p_vertex_iter;
        vertex u,v;
        bool nonEdgeFound = false;
        for (p_vertex_iter vp = vertices(g);
             vp.first != vp.second && !nonEdgeFound; ++vp.first)
        {
            for (vertex_iter w = vp.first+1;
                 w != vp.second && !nonEdgeFound; ++w)
            {
                if (!edge(*vp.first, *w, g).second)
                {
                    u = *vp.first;
                    v = *w;
                    nonEdgeFound = true;
                }
            }
        }
        Graph g1 = contract(u, v, g);
        long num = numColK(g1, k);
        Graph g2(g);
        add_edge(u,v,g2);
        num += numColK(g2, k);
        return num;
    }

    /**
     * Checks weither a graph g is planar.
     * i.e., if it can be drawn without
     * crossing edges.
     */
    template <class Graph>
    bool isPlanar(const Graph & g)
    {
        //We need to convert g into an adjacency_list because
        //the function of boost does not work correctly
        //on adjacency_matrix
        typedef boost::adjacency_list<boost::vecS, boost::vecS,
                                      boost::undirectedS> AdjList;
        typedef typename boost::graph_traits<Graph>::edge_iterator eiter;
        AdjList h(order(g));
        std::pair<eiter, eiter> ep;
        for (ep = edges(g); ep.first != ep.second; ++ep.first)
        {
            int u = source(*ep.first,g), v = target(*ep.first,g);
            add_edge(u,v,h);
        }
        return boyer_myrvold_planarity_test(h);
    }

    template <class Graph>
    std::vector<long> listEccentricities(const Graph & g)
    {
        dMatrix dist = distanceMatrix(g);
        long n = order(g);
        std::vector<long> res(n);
        for (long i = 0; i < n; i++)
        {
            long m = 0;
            for (long j = 0; j < n; j++)
            {
                if (m < dist[i][j])
                {
                    m = dist[i][j];
                }
            }
            res[i] = m;
        }
        make_heap(res.begin(), res.end());
        sort_heap(res.begin(), res.end());
        return res;
    }

    /**
     * Compute the eccentric connectivity of a graph g.
     * i.e., the sum for every node v of the products
     * between the eccentricity of v and its degree.
     * The graph needs to be connected.
     */
    template <class Graph>
    long eccentricConnectivity(const Graph & g)
    {
        long res = 0, n = order(g);
        dMatrix dist = distanceMatrix(g);
        for (long i = 0; i < n; i++)
        {
            long ecc = 0, deg = 0;
            for (long j = 0; j < n; j++)
            {
                if (edge(i,j,g).second)
                {
                    deg++;
                }
                if (dist[i][j] > ecc)
                {
                    ecc = dist[i][j];
                }
            }
            res += ecc * deg;
        }
        return res;
    }

  template <class Graph>
  long maxIndependentSet(const Graph & g) {
    int n = order(g);

    if (n <= 2) {
      if (n <= 1)
        return n;
      else if (edge(0, 1, g).second)
        return 1;
      else
        return 2;
    }

    /* Nodes in current clique candidate. */
    boost::shared_array<int> S(new int[n]);
    int i, j, s, /* Loops indices. */
      flag, /* Next node in the B&B. */
      last, /* Last node added to the clique. */
      size, /* Current max clique size. */
      conn; /* Is the current edge connected to every
               node in the current clique ? */

    for (i = 0; i < n; i++)
      S[i] = -1;

    size = 0;
    S[0] = 0;
    flag = 1;
    last = 1;
    for (s = 0; s < n - 1; s++) {
      S[0] = s;
      while (S[0] >= 0) {
        for (i = flag; i < n; i++) {
          conn = 1;
          for (j = 0; j < last && conn; j++) {
            if (edge(S[j], i, g).second)
              conn = 0;
          }
          if (conn) {
            S[last] = i;
            last++;
            flag = i + 1;
            if (last > size)
              size = last;
          }
        }
        last--;
        flag = S[last] + 1;
        S[last] = -1;
      }
    }

    return size;
  }

  template <class Graph>
  long minVertexCover(const Graph &g) {
    return order(g) - maxIndependentSet(g);
  }

  template <class Graph>
  long chromaticNumber(const Graph & g) {
    if (order(g) <= 1)
      return order(g);

    /* Lower bound. */
    int lb;
    /* Upper bound. */
    int ub;
    /* Chromatic number */
    int chi;
    /* Current number of colors used. */
    int k;

    double f = double(order(g)) / double(maxIndependentSet(g));
    lb = int(ceil(f));
    ub = detail::seq_colors(g);

    chi = lb;

    bool stop = false;
    if (lb < ub) {
      for (k = lb; k < ub && !stop; k++) {
        if (detail::is_k_colorable(g, k)) {
          chi = k;
          stop = true;
        }
      }
      if (!stop)
        chi = ub;
    }

    return chi;
  }

} //namespace phoeg

#endif
