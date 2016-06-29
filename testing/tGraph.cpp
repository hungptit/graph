#include "gtest/gtest.h"
#include <iostream>
#include <tuple>
#include <vector>

#include "fmt/format.h"

#include "graph/SparseGraph.hpp"
#include "Data.hpp"

namespace {
    template <typename IArchive, typename OArchive>
    void serialize_deserialize() {
        using index_type = size_t;
        auto data = simpleDirectedGraph<index_type>();
        auto edges = std::get<0>(data);
        auto labels = std::get<1>(data);
        auto N = std::get<2>(data);
        using EdgeData = decltype(edges)::value_type;

        std::stringstream output;

        // Serialize graph data to the output string stream
        graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, N, true);
        {
            OArchive oar(output);
            oar(cereal::make_nvp("Original_graph", g));
        }

        fmt::print("{}\n", output.str());

        // Deserialized the data
        graph::SparseGraph<index_type, decltype(edges)::value_type> g1;
        {
            IArchive iar(output);
            iar(g1);
        }
        
        // The original and deserialized graphs must be the same.
        EXPECT_TRUE(g == g1);

        // Display the serialized results.
        std::stringstream output1;
        {
            OArchive oar(output1);
            oar(cereal::make_nvp("Reconstructed_graph", g1));
        }
        fmt::print("{}\n", output1.str());
    }
}

TEST(BasicEdgeData, Positive) {
    using index_type = size_t;
    using EdgeData = graph::BasicEdgeData<index_type>;
    EdgeData anEdge{1, 2};
    std::stringstream output;
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("An edge", anEdge));
    }

    std::vector<EdgeData> edges{{1, 2}, {2, 3}, {1, 3}};
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Multiple edges", edges));
    }

    std::sort(edges.begin(), edges.end(), graph::Greater<index_type, EdgeData>());

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Sorted edges with Greater", edges));
    }

    // std::sort(edges.begin(), edges.end(), graph::Less<index_type, EdgeData>());

    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Sorted edges with Less", edges));
    }

    fmt::print("{}\n", output.str());
}

TEST(WeightedEdgeData, Positive) {
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

TEST(SparseGraph, Positive) {
    using index_type = size_t;
    auto data = simpleDirectedGraph<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    using EdgeData = decltype(edges)::value_type;

    EXPECT_TRUE(
        std::is_sorted(edges.begin(), edges.end(), graph::Less<index_type, EdgeData>()));

    std::stringstream output;
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Edges", edges), cereal::make_nvp("Labels", labels));
    }

    // Construct the graph given edge data
    graph::SparseGraph<index_type, decltype(edges)::value_type> g(edges, 6, true);
    {
        cereal::JSONOutputArchive oar(output);
        oar(cereal::make_nvp("Constructed graph", g));
    }

    fmt::print("{}\n", output.str());

    // Test edges methods
    {
        EXPECT_TRUE(g.begin(0) == 0);
        EXPECT_TRUE(g.end(0) == 3);
    }

    // Generate the dot graph
    std::string dotFile = "graph.dot";
    graph::gendot(g, labels, dotFile);
}

TEST(Serialization, Positive) {
    serialize_deserialize<cereal::BinaryInputArchive, cereal::BinaryOutputArchive>();
    serialize_deserialize<cereal::PortableBinaryInputArchive, cereal::PortableBinaryOutputArchive>();
    serialize_deserialize<cereal::JSONInputArchive, cereal::JSONOutputArchive>();
    serialize_deserialize<cereal::XMLInputArchive, cereal::XMLOutputArchive>();
}
