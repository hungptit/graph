#pragma once

#include "data_structures.hpp"
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
    bfs_preordering(const Graph &g, const std::vector<typename Graph::index_type> &vids) {
        using index_type = typename Graph::index_type;
        using EdgeData = typename Graph::edge_type;

        size_t N = g.number_of_vertexes();
        Container aQueue(vids.begin(), vids.end());
        std::vector<NodeStatus> status(N, NodeStatus::UNDISCOVERED);
        std::vector<typename Graph::index_type> results;
        results.reserve(N);

        while (!aQueue.empty()) {
            index_type currentVid = aQueue.front();
            assert(currentVid < N);
            aQueue.pop_front();

            index_type const begin = g.begin(currentVid);
            index_type const end = g.end(currentVid);

            if (status[currentVid] == NodeStatus::UNDISCOVERED) {
                status[currentVid] = NodeStatus::DISCOVERED;
                results.push_back(currentVid);
                for (index_type eidx = begin; eidx < end; ++eidx) {
                    const EdgeData anEdge = g.edge(eidx);
                    const index_type childVid = anEdge.DstId;
                    aQueue.push_back(childVid);
                }
            }
        }
        return results;
    }
}
