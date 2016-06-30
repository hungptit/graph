#include "gtest/gtest.h"
#include <iostream>
#include <tuple>
#include <vector>

#include "fmt/format.h"

#include "Data.hpp"
#include "graph/SparseGraph.hpp"

TEST(dfs_preordering, Positive) {
    using index_type = size_t;
    auto data = topologicalSortedListGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);
    std::stringstream output;
    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
    auto vids = graph::topological_sorted_list(g);
    std::vector<std::string> results;
    for (auto vid : vids) {
        results.push_back(labels[vid]);
    }

    std::vector<std::string> expectedResults{"A", "B", "C", "D", "E", "F", "G"};
    EXPECT_EQ(expectedResults, results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("topological sorted list", results));
    }

    fmt::print("{}\n", output.str());
}
