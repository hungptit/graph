#ifndef TraversalAlgorithms_hpp
#define TraversalAlgorithms_hpp

#include "DataStructures.hpp"

namespace graph {
    template <typename Graph, typename Constainer>
    struct Traverse {
      public:
        using index_type = Graph::index_type;
        void traverse(const index_type vid) {
            
        }
        
      protected:
        std::vector<int> Visited;
        std::vector<int> Processed;
        Container stack;
    }
}
#endif
