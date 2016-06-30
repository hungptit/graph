#ifndef TopologicalSortedList_hpp_
#define TopologicalSortedList_hpp_

#include "DataStructures.hpp"
#include <deque>
#include <vector>

namespace graph {

    /**
     * Return the topological sorted list of a graph using DFS strategy.
     * https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search
     *
     * @param g A directed acyclic graph
     *
     * @return The topological sorted list of a given graph.
     */
  template <typename Graph> std::deque<typename Graph::index_type> topological_sorted_list(const Graph &g) {
        using index_type = typename Graph::index_type;
        const index_type N = g.numberOfVertexes();
        std::deque<index_type> results;
        std::vector<NodeStatus> status(N, UNDISCOVERED);
        for (index_type vid = 0; vid < N; ++vid) {
            if (status[vid] == UNDISCOVERED) {
              visit(g, vid, status, results);
            }
        }
        return results;
    }

    template <typename Graph>
    void visit(const Graph &g, const typename Graph::index_type vid, std::vector<NodeStatus> &status,
               std::deque<typename Graph::index_type> &results) {
      using index_type = typename Graph::index_type;
      using edge_type = typename Graph::edge_type;
      
        if (status[vid] == VISITED) {
            return;
        }

        if (status[vid] == UNDISCOVERED) {
          status[vid] = VISITED;
          const index_type begin = g.begin(vid);
          const index_type end = g.end(vid);
          for (index_type eidx = end; eidx > begin;) {
            const edge_type anEdge = g.edge(--eidx);
            const index_type childVid = anEdge.DstId;
            visit(g, childVid, status, results);
          }
          status[vid] = DISCOVERED;
          results.push_front(vid);
        }
    }
}
#endif
