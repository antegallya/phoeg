#ifndef GRAPH6_HPP
#define GRAPH6_HPP

#include <string>
#include "graph.hpp"
#include "invariants.hpp"

namespace phoeg {

  namespace detail {

    /**
     * Computes the length of the string encoding
     * a graph with n vertices in the graph6 format.
     */
    long lengthGraph6(long n)
    {
      long res;
      //How many bytes to encode the order
      if (n < 63) {
        res = 1;
      }
      else if (n < 258048) {
        res = 4;
      }
      else if (n < 68719476736) {
        res = 8;
      }
      //How many edges
      long k = (n * (n-1)) / 2;
      //How many bytes to encode the size
      res += ceil(k / 6.);
      return res;
    }

    /**
     * Returns the first bytes of the string
     * encoding the order of the graph in the
     * graph6 format.
     */
    std::string encodeOrder(long n)
    {
      std::string result;
      if (n < 63) {
        result += (char) n+63;
      }
      else if (n < 258048) {
        result += (char) 126;
        for (int i = 0; i < 3; i++) {
          result += (char) (n >> i*6) % 64 + 63;
        }
      }
      else if (n < 68719476736) {
        result += (char) 126;
        result += (char) 126;
        for (int i = 0; i < 4; i++) {
          result += (char) (n >> i*6) % 64 + 63;
        }
      }
      return result;
    }

    /**
     * Decodes the order of the graph from it's graph6 format
     * and removes the encoding of the order from the string.
     */
    long decodeOrderGraph6(std::string & g6) {
      long n = 0;
      if (g6.size() > 0 && g6[0] < 126) {
        n = g6[0] - 63;
        g6.erase(g6.begin(),g6.begin()+1);
      }
      else if (g6.size() > 2 && g6[0] == 126 && g6[1] < 126) {
        n = (g6[1]-63)<<12 + (g6[2]-63)<<6 + (g6[3]-63);
        g6.erase(g6.begin(),g6.begin()+4);
      }
      else if (g6.size() > 6 && g6[0] == 126 && g6[1] == 126) {
        n = (g6[2]-63)<<30 + (g6[3]-63)<<24 + (g6[4]-63)<<18 +
          (g6[5]-63)<<12 + (g6[6]-63)<<6 + (g6[7]-63);
        g6.erase(g6.begin(),g6.begin()+8);
      }
      return n;
    }

  } // namespace detail


  /**
   * Converts a graph to graph6 format
   */
  std::string convertToGraph6(const Graph & g) {
    std::string g6 = detail::encodeOrder(order(g));
    if (numEdges(g) == 0) {
      long l = detail::lengthGraph6(order(g));
      for (long p = g6.size(); p < l; p++) {
        g6 += '?';
      }
    }
    else {
      int l = 0;
      char r = 0;
      std::pair<vertex_iter,vertex_iter> u = vertices(g);
      //We skip the first vertex since we want the inferior
      //half of the adjacency matrix
      u.first++;
      for (; u.first != u.second ; u.first++) {
        for (std::pair<vertex_iter,vertex_iter> v = vertices(g);
             v.first != u.first; ++v.first) {
          if (l == 6) {
            g6 += (char) r+63;
            l = 0;
            r = 0;
          }
          //We add a bit if there is an edge
          bool isEdge = boost::edge(*u.first, *v.first, g).second;
          r = (r << 1) + (isEdge ? 1 : 0);
          l++;
        }
      }
      //We might have to add 0's at the end of the last byte
      //if it wasn't long enough
      if (l <= 6) {
        r = r << (6-l);
        g6 += (char) r+63;
      }
    }
    return g6;
  }

  /**
   * Returns the graph stored in the string s
   * encoded in the graph6 format.
   */
  Graph convertFromGraph6(const std::string & s) {
    std::string g6(s);
    long n = detail::decodeOrderGraph6(g6);
    Graph g(n);
    if (g6.size() > 0) {
      char v = g6[0]-63;
      int l = 5;
      long p = 0;
      for (long i = 1; i < n; i++) {
        for (long j = 0; j < i; j++) {
          if (l < 0) {
            l = 5;
            p++;
            v = g6[p]-63;
          }
          //We get the value of the bit in position l
          if ((v % (1<<(l+1)))>>l) {
            add_edge(i,j,g);
          }
          l--;
        }
      }
    }
    return g;
  }

}

#endif
