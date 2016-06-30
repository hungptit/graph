#ifndef Data_hpp_
#define Data_hpp_

#include <string>
#include <tuple>
#include <vector>

#include "graph/DataStructures.hpp"

namespace {
    template <typename index_type> auto simpleDirectedGraph() {
        using EdgeData = graph::BasicEdgeData<index_type>;
        std::vector<graph::BasicEdgeData<index_type>> edges = {{0, 1}, {0, 2}, {0, 3}, {1, 4},
                                                               {1, 5}, {2, 6}, {3, 5}};
        std::vector<std::string> labels = {"A", "B", "C", "D", "E", "F", "G"};
        std::sort(edges.begin(), edges.end(), graph::Less<index_type, EdgeData>());
        return std::make_tuple(edges, labels, 7);
    }

    template <typename index_type> auto topologicalSortedListGraph() {
        using EdgeData = graph::BasicEdgeData<index_type>;
        std::vector<graph::BasicEdgeData<index_type>> edges = {
            {0, 3}, {1, 3}, {1, 4}, {2, 4}, {3, 5}, {3, 6}, {4, 6}, {3, 7}, {2, 7}};
        std::vector<std::string> labels = {"A", "B", "C", "D", "E", "F", "G", "H"};
        std::sort(edges.begin(), edges.end(), graph::Less<index_type, EdgeData>());
        return std::make_tuple(edges, labels, 8);
    }
}

#endif