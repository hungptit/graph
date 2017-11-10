#include <iostream>
#include <tuple>
#include <vector>

#include "fmt/format.h"
#include "graph/graph.hpp"
#include "graph/DataStructures.hpp"
#include "Data.hpp"

// Cereal
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

namespace {
    template <typename IArchive, typename OArchive> void serialize_deserialize() {
        using index_type = size_t;
        auto data = simpleDirectedGraph<index_type>();
        auto edges = std::get<0>(data);
        auto labels = std::get<1>(data);
        auto N = std::get<2>(data);
        std::stringstream output;

        using edge_container = decltype(edges);
        using edge_type = typename edge_container::value_type;

        // Serialize graph data to the output string stream
        graph::SparseGraph<index_type, edge_type> g(edges, N, true);
        {
            OArchive oar(output);
            oar(cereal::make_nvp("Original_graph", g));
        }

        fmt::print("{}\n", output.str());

        // Deserialized the data
        graph::SparseGraph<index_type, typename decltype(edges)::value_type> g1;
        {
            IArchive iar(output);
            iar(g1);
        }

        // The original and deserialized graphs must be the same.
        REQUIRE(g == g1);

        // Display the serialized results.
        std::stringstream output1;
        {
            OArchive oar(output1);
            oar(cereal::make_nvp("Reconstructed_graph", g1));
        }
        fmt::print("{}\n", output1.str());
    }
}

TEST_CASE("BasicEdgeData", "Positive") {
    using index_type = size_t;
    using EdgeData = graph::BasicEdgeData<index_type>;
    EdgeData anEdge{1, 2};
    std::stringstream output;
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("An edge", anEdge));
    }

    std::vector<EdgeData> edges{{1, 2}, {2, 3}, {1, 3}};
    std::sort(edges.begin(), edges.end());

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Multiple edges", edges));
    }

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Sorted edges with Less", edges));
    }

    fmt::print("{}\n", output.str());
}

TEST_CASE("BasicEdgeDataHash", "Positive") {
    using index_type = size_t;
    using EdgeData = graph::BasicEdgeData<index_type>;
    std::stringstream output;
    std::vector<EdgeData> data{{1, 2}, {2, 3}, {1, 2}, {1, 3}};
    std::sort(data.begin(), data.end());

    std::unordered_set<EdgeData> edges(data.begin(), data.end());
    std::set<EdgeData> aSet(data.begin(), data.end());

    REQUIRE(edges.size() == 3);

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("data", data));
    }

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("std::unordered_set", edges));
    }

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("std::set", aSet));
    }

    fmt::print("{}\n", output.str());

    EdgeData x{1, 2};
    EdgeData y{1, 3};
    EdgeData z{2, 3};
    REQUIRE(x < y);
    REQUIRE(x < z);
    REQUIRE(!(y < x));
    REQUIRE(x == x);
    REQUIRE(!(x == y));
}

TEST_CASE("WeightedEdgeDataHash", "Positive") {
    using index_type = size_t;
    using weight_type = int;
    using EdgeData = graph::WeightedEdgeData<index_type, weight_type>;
    std::vector<EdgeData> data{{1, 2, 3}, {2, 3, 1}, {1, 2, 2}, {1, 3, 0}, {1, 2, 3}};
    std::unordered_set<EdgeData> edges(data.begin(), data.end());

    REQUIRE(edges.size() == 4);

    std::stringstream output;
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Raw edges data", edges));
    }

    fmt::print("{}\n", output.str());

    EdgeData x{1, 2, 1};
    EdgeData y{1, 3, 2};
    EdgeData z{2, 3, 1};
    EdgeData t{1, 2, 3};
    REQUIRE(x < y);
    REQUIRE(x < z);
    REQUIRE(x < t);
    REQUIRE(!(y < x));
    REQUIRE(x == x);
    REQUIRE(!(x == y));
}

TEST_CASE("WeightedEdgeData", "Positive") {
    using index_type = size_t;
    using weight_type = int;
    using EdgeData = graph::WeightedEdgeData<index_type, weight_type>;
    EdgeData anEdge{1, 2, 2};
    std::stringstream output;
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("A weighted edge", anEdge));
    }

    std::vector<EdgeData> edges{{1, 2, 3}, {2, 3, 4}, {1, 3, 3}};
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Multiple weighted edges", edges));
    }

    fmt::print("{}\n", output.str());
}

TEST_CASE("SparseGraph", "Positive") {
    using index_type = size_t;
    auto data = simpleDirectedGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    using edge_type = decltype(edges)::value_type;

    REQUIRE(std::is_sorted(edges.begin(), edges.end()));

    std::stringstream output;
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Edges", edges), cereal::make_nvp("Labels", labels));
    }

    // Construct the graph given edge data
    graph::SparseGraph<index_type, edge_type> g(edges, 6, true);
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Constructed graph", g));
    }

    fmt::print("{}\n", output.str());

    // Test edges methods
    {
        REQUIRE(g.begin(0) == 0);
        REQUIRE(g.end(0) == 3);
    }

    // Generate the dot graph
    std::string dotFile = "graph.dot";
    graph::gendot(g, labels, dotFile);
}

TEST_CASE("Serialization", "") {
    serialize_deserialize<cereal::BinaryInputArchive, cereal::BinaryOutputArchive>();
    serialize_deserialize<cereal::PortableBinaryInputArchive,
                          cereal::PortableBinaryOutputArchive>();
    serialize_deserialize<cereal::JSONInputArchive, cereal::JSONOutputArchive>();
    serialize_deserialize<cereal::XMLInputArchive, cereal::XMLOutputArchive>();
}
