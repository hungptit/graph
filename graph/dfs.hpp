#pragma once

#include <deque>
#include <vector>

#include "data_structures.hpp"

// TODO: Compare the performance of non-recursive and recursive implementations.

namespace graph {
    /**
     * Non-recursive implementation of the DFS algorithms
     *
     * @param g
     * @param vid
     *
     * @return
     */
    template <typename Container, typename Graph>
    std::vector<typename Graph::index_type>
    dfs_preordering(const Graph &g, const std::vector<typename Graph::index_type> &vids) {
        using index_type = typename Graph::index_type;
        using edge_type = typename Graph::edge_type;
        const size_t N = g.number_of_vertexes();
        Container stack(vids.cbegin(), vids.cend());
        std::vector<NodeStatus> status(N, NodeStatus::UNDISCOVERED);
        std::vector<typename Graph::index_type> results;
        results.reserve(N);

        while (!stack.empty()) {
            index_type currentVid = stack.back();
            assert(currentVid < N); // Validate the currentVid
            stack.pop_back();
            if (status[currentVid] == NodeStatus::UNDISCOVERED) {
                index_type const begin = g.begin(currentVid);
                index_type const end = g.end(currentVid);
                status[currentVid] = NodeStatus::DISCOVERED;
                // Mark the current vertex as DISCOVERED then traverse to all of
                // its children.

                for (index_type eidx = end; eidx > begin;) {
                    const edge_type anEdge = g.edge(--eidx);
                    const index_type childVid = anEdge.DstId;
                    stack.push_back(childVid);
                }
                results.push_back(currentVid);
            }
        }
        return results;
    }

    // TODO: Need to check this algorithm.
    template <typename Container, typename Graph>
    void dfs_postordering_back(const Graph &g,
                               const std::vector<typename Graph::index_type> &vids,
                               std::vector<NodeStatus> &status,
                               std::vector<typename Graph::index_type> &results) {
        using index_type = typename Graph::index_type;
        using edge_type = typename Graph::edge_type;
        Container stack(vids.cbegin(), vids.cend());

        while (!stack.empty()) {
            index_type currentVid = stack.back();
            assert(currentVid < static_cast<index_type>(g.numberOfVertexes()));
            if (status[currentVid] == NodeStatus::UNDISCOVERED) {
                index_type const begin = g.begin(currentVid);
                index_type const end = g.end(currentVid);
                status[currentVid] = NodeStatus::VISITED;
                for (index_type eidx = end; eidx > begin;) {
                    const edge_type anEdge = g.edge(--eidx);
                    const index_type childVid = anEdge.DstId;
                    stack.push_back(childVid);
                }
                continue;
            }

            stack.pop_back();
            if (status[currentVid] == NodeStatus::VISITED) {
                results.push_back(currentVid);
                status[currentVid] = NodeStatus::DISCOVERED;
            } else {
                // There is a loop in a given graph.
            }
        }
    }

    // This function assume that a given graph is DAG.
    // TODO: Need to check this algorithm.
    template <typename Container, typename Graph>
    void dfs_postordering_front(const Graph &g,
                                const std::vector<typename Graph::index_type> &vids,
                                std::vector<NodeStatus> &status,
                                std::deque<typename Graph::index_type> &results) {
        using index_type = typename Graph::index_type;
        using edge_type = typename Graph::edge_type;

        Container stack(vids.cbegin(), vids.cend());

        while (!stack.empty()) {
            index_type currentVid = stack.back();
            assert(currentVid < static_cast<index_type>(g.numberOfVertexes()));

            if (status[currentVid] == NodeStatus::UNDISCOVERED) {
                index_type const begin = g.begin(currentVid);
                index_type const end = g.end(currentVid);
                status[currentVid] = NodeStatus::VISITED;
                for (index_type eidx = end; eidx > begin;) {
                    const edge_type anEdge = g.edge(--eidx);
                    const index_type childVid = anEdge.DstId;
                    stack.push_back(childVid);
                }
                continue;
            }

            stack.pop_back();
            if (status[currentVid] == NodeStatus::VISITED) {
                results.push_front(currentVid);
                status[currentVid] = NodeStatus::DISCOVERED;
            } else {
                // throw(std::runtime_error("Found a loop in a given graph"));
            }
        }
    }

    template <typename Container, typename Graph>
    std::vector<typename Graph::index_type>
    dfs_postordering(const Graph &g, const std::vector<typename Graph::index_type> &vids) {
        using index_type = typename Graph::index_type;
        size_t N = g.number_of_vertexes();
        std::vector<NodeStatus> status(N, NodeStatus::UNDISCOVERED);
        std::vector<index_type> results;
        results.reserve(N);
        dfs_postordering_back<Container>(g, vids, status, results);
        return results;
    }

    /**
     * Recursive implementation of the DFS algorithm.
     *
     * @param g
     * @param vid
     * @param status
     * @param results
     */
    template <typename Graph>
    void dfs_recursive_preordering(const Graph &g, const typename Graph::index_type vid,
                                   std::vector<NodeStatus> &status,
                                   std::vector<typename Graph::index_type> &results) {
        using index_type = typename Graph::index_type;
        using edge_type = typename Graph::edge_type;
        assert(vid < static_cast<index_type>(g.number_of_vertexes()));
        if (status[vid] == NodeStatus::UNDISCOVERED) {
            if (status[vid] == NodeStatus::UNDISCOVERED) {
                index_type const begin = g.begin(vid);
                index_type const end = g.end(vid);
                status[vid] = NodeStatus::DISCOVERED;
                results.push_back(vid);
                for (index_type eidx = begin; eidx < end; ++eidx) {
                    const edge_type anEdge = g.edge(eidx);
                    const index_type childVid = anEdge.DstId;
                    if (status[childVid] == NodeStatus::UNDISCOVERED) {
                        dfs_recursive_preordering(g, childVid, status, results);
                    }
                }
            }
        }
    }

    template <typename Graph>
    void dfs_recursive_postordering(const Graph &g, const typename Graph::index_type vid,
                                    std::vector<NodeStatus> &status,
                                    std::vector<typename Graph::index_type> &results) {
        using index_type = typename Graph::index_type;
        using edge_type = typename Graph::edge_type;
        assert(vid < static_cast<index_type>(g.number_of_vertexes()));
        if (status[vid] == NodeStatus::UNDISCOVERED) {
            index_type const begin = g.begin(vid);
            index_type const end = g.end(vid);
            status[vid] = NodeStatus::DISCOVERED;
            for (index_type eidx = begin; eidx < end; ++eidx) {
                const edge_type anEdge = g.edge(eidx);
                const index_type childVid = anEdge.DstId;
                if (status[childVid] == NodeStatus::UNDISCOVERED) {
                    dfs_recursive_postordering(g, childVid, status, results);
                }
            }
            results.push_back(vid);
        }
    }
}
