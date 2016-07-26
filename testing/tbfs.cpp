#include "gtest/gtest.h"
#include <iostream>
#include <tuple>
#include <vector>

#include "fmt/format.h"

#include "graph/SparseGraph.hpp"
#include "Data.hpp"

std::vector<std::string> expectedResultsPreOrdering{ "A", "B", "C", "E", "D", "F", "G" };
std::vector<std::string> expectedResultsPostOrdering{"A", "B", "C", "D", "E", "F", "G"};

TEST(bfs_preordering, Positive) {
    using index_type = size_t;
    auto data = simpleDirectedGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);

    using EdgeData = decltype(edges)::value_type;

    std::stringstream output;

    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
    index_type rootVid = 0;
    auto vids = graph::bfs_preordering<std::deque<index_type>>(g, rootVid);
    std::vector<std::string> results;
    for (auto vid : vids) {
        results.push_back(labels[vid]);
    }

    EXPECT_EQ(expectedResultsPreOrdering, results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("dfs results", results));
    }

    fmt::print("{}\n", output.str());
}
