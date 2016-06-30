#ifndef DFS_hpp_
#define DFS_hpp_

#include "DataStructures.hpp"

#include <vector>

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
    dfs_preordering(const Graph &g, const typename Graph::index_type vid) {
        using index_type = typename Graph::index_type;
        using EdgeData = typename Graph::edge_type;

        size_t N = g.numberOfVertexes();
        assert(vid < N);

        Container stack{vid};
        std::vector<NodeStatus> status(N, UNDISCOVERED);
        std::vector<typename Graph::index_type> results;
        results.reserve(N);

        while (!stack.empty()) {
            index_type currentVid = stack.back();
            assert(currentVid < N);
            stack.pop_back();
            if (status[currentVid] == UNDISCOVERED) {
                index_type const begin = g.begin(currentVid);
                index_type const end = g.end(currentVid);
                status[currentVid] = DISCOVERED;
                for (index_type eidx = end; eidx > begin;) {
                    const EdgeData anEdge = g.edge(--eidx);
                    const index_type childVid = anEdge.DstId;
                    stack.push_back(childVid);
                }
                results.push_back(currentVid);
            }
        }
        return results;
    }

    template <typename Container, typename Graph>
    std::vector<typename Graph::index_type>
    dfs_postordering(const Graph &g, const typename Graph::index_type vid) {
        using index_type = typename Graph::index_type;
        using EdgeData = typename Graph::edge_type;

        size_t N = g.numberOfVertexes();
        assert(vid < N);

        Container stack{vid};
        std::vector<NodeStatus> status(N, UNDISCOVERED);
        std::vector<typename Graph::index_type> results;
        results.reserve(N);

        while (!stack.empty()) {
            index_type currentVid = stack.back();
            assert(currentVid < N);
            
            if (status[currentVid] == UNDISCOVERED) {
                index_type const begin = g.begin(currentVid);
                index_type const end = g.end(currentVid);
                status[currentVid] = VISITED;
                for (index_type eidx = end; eidx > begin;) {
                    const EdgeData anEdge = g.edge(--eidx);
                    const index_type childVid = anEdge.DstId;
                    stack.push_back(childVid);
                }
                continue;
            } 
            
            stack.pop_back();
            if (status[currentVid] == VISITED) {
                results.push_back(currentVid);
                status[currentVid] = DISCOVERED;
            }
        }
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
        using EdgeData = typename Graph::edge_type;

        size_t N = g.numberOfVertexes();
        assert(vid < N);

        if (status[vid] == UNDISCOVERED) {
            index_type const begin = g.begin(vid);
            index_type const end = g.end(vid);
            status[vid] = DISCOVERED;
            results.push_back(vid);
            for (index_type eidx = begin; eidx < end; ++eidx) {
                const EdgeData anEdge = g.edge(eidx);
                const index_type childVid = anEdge.DstId;
                if (status[childVid] == UNDISCOVERED) {
                    dfs_recursive_preordering(g, childVid, status, results);
                }
            }
        }
    }

    template <typename Graph>
    void dfs_recursive_postordering(const Graph &g, const typename Graph::index_type vid,
                                    std::vector<NodeStatus> &status,
                                    std::vector<typename Graph::index_type> &results) {
        using index_type = typename Graph::index_type;
        using EdgeData = typename Graph::edge_type;

        size_t N = g.numberOfVertexes();
        assert(vid < N);

        if (status[vid] == UNDISCOVERED) {
            index_type const begin = g.begin(vid);
            index_type const end = g.end(vid);
            status[vid] = DISCOVERED;
            for (index_type eidx = begin; eidx < end; ++eidx) {
                const EdgeData anEdge = g.edge(eidx);
                const index_type childVid = anEdge.DstId;
                if (status[childVid] == UNDISCOVERED) {
                    dfs_recursive_postordering(g, childVid, status, results);
                }
            }
            results.push_back(vid);
        }
    }
}

#endif
