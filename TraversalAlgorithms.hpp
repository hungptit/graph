#ifndef TraversalAlgorithms_hpp
#define TraversalAlgorithms_hpp

#include "DataStructures.hpp"
#include <vector>

namespace graph {
    template <typename Graph>
    void dfs_inorder(const Graph &g, const typename Graph::index_type vid) {
      using index_type = typename Graph::index_type;
        size_t N = g.Vertexes.size() - 1;
        std::vector<index_type> stack{vid};
        std::vector<int> visited(N, 0);
        while (!stack.empty()) {
            index_type currentVid = stack.back();
            std::array<index_type, 2> bound = g.edges(currentVid);
            for (auto vidx = bound[0]; vidx < bound[1]; ++vidx) {
            }
        }
    }
}
#endif
