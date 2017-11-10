#include <iostream>
#include <tuple>
#include <vector>

#include "fmt/format.h"

#include "graph/SparseGraph.hpp"
#include "Data.hpp"

#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/deque.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/set.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal/types/vector.hpp"

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

std::vector<std::string> expectedResultsPreOrdering{ "A", "B", "C", "E", "D", "F", "G" };
std::vector<std::string> expectedResultsPostOrdering{"A", "B", "C", "D", "E", "F", "G"};

TEST_CASE("bfs_preordering", "Positive") {
    using index_type = size_t;
    auto data = simpleDirectedGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    auto N = std::get<2>(data);
    std::stringstream output;

    graph::SparseGraph<index_type, decltype(edges)::value_type> g(std::get<0>(data), N, true);
    index_type rootVid = 0;
    auto vids = graph::bfs_preordering<std::deque<index_type>>(g, {rootVid});
    std::vector<std::string> results;
    for (auto vid : vids) {
        results.push_back(labels[vid]);
    }

    REQUIRE(expectedResultsPreOrdering == results);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("dfs results", results));
    }

    fmt::print("{}\n", output.str());
}
