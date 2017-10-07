#pragma once
#include <string>
#include <tuple>
#include <vector>

#include "graph/DataStructures.hpp"

namespace {
    // The test graph is taken from this wiki page:
    // https://en.wikipedia.org/wiki/Depth-first_search
    template <typename index_type> auto simpleDirectedGraph() {
        using EdgeData = graph::BasicEdgeData<index_type>;
        std::vector<EdgeData> edges = {{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 6}, {5, 4}};
        std::vector<std::string> labels = {"A", "B", "C", "D", "E", "F", "G"};
        std::sort(edges.begin(), edges.end());
        return std::make_tuple(edges, labels, 7);
    }

    template <typename index_type> auto topologicalSortedListGraph() {
        using edge_type = graph::BasicEdgeData<index_type>;
        std::vector<edge_type> edges = {{0, 3}, {1, 3}, {1, 4}, {2, 4}, {3, 5},
                                        {3, 6}, {4, 6}, {3, 7}, {2, 7}};
        std::vector<std::string> labels = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
        std::sort(edges.begin(), edges.end());
        return std::make_tuple(edges, labels, 9);
    }

    template <typename index_type> auto topologicalSortedListGraphWithLoop() {
        using edge_type = graph::BasicEdgeData<index_type>;
        std::vector<edge_type> edges = {{0, 3}, {1, 3}, {1, 4}, {2, 4}, {3, 5},
                                        {3, 6}, {4, 6}, {3, 7}, {2, 7}, {7, 1}};
        std::vector<std::string> labels = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
        std::sort(edges.begin(), edges.end());
        return std::make_tuple(edges, labels, 9);
    }
} // namespace
