#ifndef Graph_BFS_hpp_
#define Graph_BFS_hpp_

#include "DataStructures.hpp"
#include <vector>

namespace graph {

    /**
     * Non-recursive implementation of the BFS algorithms.
     *
     * @param g
     * @param vid
     *
     * @return
     */
    template <typename Container, typename Graph>
    std::vector<typename Graph::index_type>
    bfs_preordering(const Graph &g, const typename Graph::index_type vid) {
        using index_type = typename Graph::index_type;
        using EdgeData = typename Graph::edge_type;

        size_t N = g.numberOfVertexes();
        assert(vid < N);

        Container stack{vid};
        std::vector<NodeStatus> status(N, UNDISCOVERED);
        std::vector<typename Graph::index_type> results;
        results.reserve(N);

        while (!stack.empty()) {
            index_type currentVid = stack.front();
            assert(currentVid < N);
            stack.pop_front();

            index_type const begin = g.begin(currentVid);
            index_type const end = g.end(currentVid);

            if (status[currentVid] == UNDISCOVERED) {
                status[currentVid] = DISCOVERED;
                results.push_back(currentVid);
                for (index_type eidx = begin; eidx < end; ++eidx) {
                    const EdgeData anEdge = g.edge(eidx);
                    const index_type childVid = anEdge.DstId;
                    stack.push_back(childVid);
                }
            }
        }
        return results;
    }
}
#endif
