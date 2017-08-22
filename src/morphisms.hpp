#ifndef MORPHISMS_H
#define MORPHISMS_H

#include <boost/graph/vf2_sub_graph_iso.hpp>

/* From Graphedron gphgraphalgo.cpp file. */
struct vf2_find_stop_callback {
    bool * _found;

    vf2_find_stop_callback(bool & found)
    {
        _found = &found;
    }

    template <typename CorrespondenceMap1To2,
              typename CorrespondenceMap2To1>
    bool operator()(CorrespondenceMap1To2, CorrespondenceMap2To1) const
    {
        /* Use a side effet to tell the caller that a mapping was found because
           the default return scheme for vf2_subgraph_mono seems to be buggy. */
        *_found = true;
        return false;
    }
};

/* Graphs have to be stored as an adjacency list for this function to work. */
template <class Graph>
bool subgraphIso(const Graph & _smallGraph, const Graph & _bigGraph)
{
    bool found = false;
    boost::vf2_subgraph_iso(_smallGraph, _bigGraph,
                            vf2_find_stop_callback(found));
    return found;
}

/* Graphs have to be stored as an adjacency list for this function to work. */
template <class Graph>
bool subgraphMono(const Graph & _smallGraph, const Graph & _bigGraph)
{
    bool found = false;
    boost::vf2_subgraph_mono(_smallGraph, _bigGraph,
                             vf2_find_stop_callback(found));
    return found;
}

#endif
