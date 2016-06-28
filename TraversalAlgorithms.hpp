#ifndef TraversalAlgorithms_hpp
#define TraversalAlgorithms_hpp

#include "DataStructures.hpp"
#include <vector>

namespace graph {
    template <typename Graph>
    std::vector<typename Graph::index_type> dfs_inorder(const Graph &g,
                                                        const typename Graph::index_type vid) {
        using index_type = typename Graph::index_type;
        size_t N = g.Vertexes.size() - 1;
        std::vector<index_type> stack{vid};
        std::vector<int> visited(N, 0);
        std::vector<int> processed(N, 0);
        std::vector<typename Graph::index_type> results;
        results.reserve(N);
        while (!stack.empty()) {
            index_type currentVid = stack.back();
            stack.pop_back();
            std::array<index_type, 2> bound = g.edges(currentVid);
            fmt::print("currentVid = {0} -> visited = {1} and processed = {2}\n", currentVid,
                       visited[currentVid], processed[currentVid]);
            if (!visited[currentVid]) {
                fmt::print("Visit {}\n", currentVid);
                for (auto eidx = bound[0]; eidx < bound[1]; ++eidx) {
                    auto anEdge = g.edge(eidx);
                    stack.push_back(anEdge.DstId);
                }
                visited[currentVid] = 1;
            } else if (!processed[vid]) {
                fmt::print("Process {}\n", currentVid);
                processed[currentVid] = 1;
                results.push_back(currentVid);
            }
        }
        return results;
    }
}
#endif
