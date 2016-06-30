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

    std::vector<std::string> expectedResults{"C", "B", "E", "A", "D", "I", "H", "G", "F"};
    EXPECT_EQ(expectedResults, results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("topological sorted list", vids));
    }

    fmt::print("{}\n", output.str());

    // std::vector<std::string> newLabels(N, "");
    // for (auto idx = 0; idx < N; ++idx) {
    //     newLabels[idx] = std::to_string(idx);
    // }

    std::string dotFile = "topological_sorted_list_graph.dot";
    graph::gendot(g, labels, dotFile);
}
