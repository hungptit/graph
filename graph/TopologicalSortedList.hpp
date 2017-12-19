#pragma once
#include "DataStructures.hpp"
#include "DFS.hpp"
#include <deque>
#include <vector>

namespace graph {

    /**
     * Return the topological sorted list of a graph using DFS strategy.
     * https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search
     *
     * @note This is a non-recursive implementation.
     * 
     * @param g A directed acyclic graph
     * 
     * @return The topological sorted list of a given graph.
     */
    template <typename Stack, typename Graph>
    std::deque<typename Graph::index_type> topological_sorted_list(const Graph &g) {
        using index_type = typename Graph::index_type;
        const index_type N = g.number_of_vertexes();
        std::deque<index_type> results;
        std::vector<NodeStatus> status(N, UNDISCOVERED);

        // TODO: Only push independent nodes into results.
        for (index_type vid = 0; vid < N; ++vid) {
            if (g.begin(vid) == g.end(vid)) {
                results.push_front(vid);
                status[vid] = DISCOVERED;
            }
        }

        for (index_type vid = 0; vid < N; ++vid) {
            if (status[vid] == UNDISCOVERED) {
                dfs_postordering_front<Stack>(g, {vid}, status, results);
            }
        }
        return results;
    }
}
