#include <iostream>
#include <tuple>
#include <vector>
#include <iterator>

#include "fmt/format.h"

#include "Data.hpp"
#include "graph/SparseGraph.hpp"

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

TEST_CASE("use_dfs_postordering", "Positive") {
    using index_type = size_t;
    auto data = topologicalSortedListGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);
    std::stringstream output;
    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
    auto vids = graph::topological_sorted_list<std::vector<index_type>>(g);
    std::vector<std::string> results;
    for (auto vid : vids) {
        results.push_back(labels[vid]);
    }

    std::vector<std::string> expectedResults{"C", "B", "E", "A", "D", "I", "H", "G", "F"};
    REQUIRE(expectedResults == results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("topological sorted list", vids));
    }

    fmt::print("{}\n", output.str());

    std::string dotFile = "topological_sorted_list_graph.dot";
    graph::gendot(g, labels, dotFile);
}

TEST_CASE("use_dfs_postordering_neg", "Negative") {
    using index_type = size_t;
    auto data = topologicalSortedListGraphWithLoop<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);
    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
    std::string dotFile = "topological_sorted_list_graph_with_loop.dot";
    graph::gendot(g, labels, dotFile);
    std::stringstream output;
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("A directed graph with loop", g));
    }
    fmt::print("{}\n", output.str());
    
    REQUIRE_NOTHROW(graph::topological_sorted_list<std::vector<index_type>>(g));
    REQUIRE_THROWS(throw(std::runtime_error("My error!")));
}

