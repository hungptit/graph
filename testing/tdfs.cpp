#include "gtest/gtest.h"
#include <iostream>
#include <tuple>
#include <vector>

#include "fmt/format.h"

#include "graph/DataStructures.hpp"
#include "graph/Operators.hpp"
#include "graph/SparseGraph.hpp"
#include "graph/TraversalAlgorithms.hpp"
#include "graph/Utils.hpp"

#include "Data.hpp"

TEST(dfs_preordering, Positive) {
    using index_type = size_t;
    auto data = simpleDirectedGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);

    using EdgeData = decltype(edges)::value_type;

    std::stringstream output;

    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
    index_type rootVid = 0;
    auto vids = graph::dfs_preordering<std::vector<index_type>>(g, rootVid);
    std::vector<std::string> results;
    for (auto vid : vids) {
        results.push_back(labels[vid]);
    }

    std::vector<std::string> expectedResults{ "A", "B", "E", "F", "C", "G", "D" };
    EXPECT_EQ(expectedResults, results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("dfs results", results));
    }

    fmt::print("{}\n", output.str());
}

TEST(dfs_postordering, Positive) {
    using index_type = size_t;
    auto data = simpleDirectedGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);

    using EdgeData = decltype(edges)::value_type;

    std::stringstream output;

    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
    index_type rootVid = 0;
    auto vids = graph::dfs_postordering<std::vector<index_type>>(g, rootVid);
    std::vector<std::string> results;
    for (auto vid : vids) {
        results.push_back(labels[vid]);
    }

    std::vector<std::string> expectedResults{ "E", "F", "B", "G", "C", "D", "A" };
    EXPECT_EQ(expectedResults, results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("dfs results", results));
    }

    fmt::print("{}\n", output.str());
}

TEST(dfs_recursive_preordering, Positive) {
    using index_type = size_t;
    auto data = simpleDirectedGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);

    using EdgeData = decltype(edges)::value_type;

    std::stringstream output;

    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
    index_type rootVid = 0;

    std::vector<graph::NodeStatus> status(N, graph::UNDISCOVERED);
    std::vector<index_type> vids;
    vids.reserve(N);

    graph::dfs_recursive_preordering(g, rootVid, status, vids);
    std::vector<std::string> results;
    for (auto vid : vids) {
        results.push_back(labels[vid]);
    }

    std::vector<std::string> expectedResults{ "A", "B", "E", "F", "C", "G", "D" };
    EXPECT_EQ(expectedResults, results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("dfs_recursive results", results));
    }

    fmt::print("{}\n", output.str());
}

TEST(dfs_recursive_postordering, Positive) {
    using index_type = size_t;
    auto data = simpleDirectedGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);

    using EdgeData = decltype(edges)::value_type;

    std::stringstream output;

    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
    index_type rootVid = 0;

    std::vector<graph::NodeStatus> status(N, graph::UNDISCOVERED);
    std::vector<index_type> vids;
    vids.reserve(N);

    graph::dfs_recursive_postordering(g, rootVid, status, vids);
    std::vector<std::string> results;
    for (auto vid : vids) {
        results.push_back(labels[vid]);
    }

    std::vector<std::string> expectedResults{ "E", "F", "B", "G", "C", "D", "A" };
    EXPECT_EQ(expectedResults, results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("dfs_recursive_postordering results", results));
    }

    fmt::print("{}\n", output.str());
}