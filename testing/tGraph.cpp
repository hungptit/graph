#include "gtest/gtest.h"
#include <iostream>
#include <tuple>
#include <vector>

#include "fmt/format.h"

#include "graph/DataStructures.hpp"
#include "graph/Operators.hpp"
#include "graph/SparseGraph.hpp"
#include "graph/TraversalAlgorithms.hpp"

namespace {
    template <typename index_type> auto createGraphData() {
        using EdgeData = graph::BasicEdgeData<index_type>;
        std::vector<graph::BasicEdgeData<index_type>> edges = {{0, 1}, {0, 3}, {0, 2}, {1, 3},
                                                               {1, 4}, {2, 5}, {4, 3}};
        std::vector<std::string> labels = {"A", "B", "C", "D", "E", "F", "G"};
        std::sort(edges.begin(), edges.end(), graph::Less<index_type, EdgeData>());
        return std::make_tuple(edges, labels);
    }

    // template <typename index_type, typename edge_type>
    // std::vector<graph::WeightedEdgeData<index_type, edge_type>> createGraphData() {
    //     using EdgeData = graph::WeightedEdgeData<index_type, edge_type>;
    //     std::vector<EdgeData> edges = {{1, 2, 3}};
    //     std::vector<std::string> labels = {"A", "B", "C", "D", "E", "F", "G"};
    //     std::sort(edges.begin(), edges.end(), graph::Less<index_type, EdgeData>());
    //     return std::make_tuple(edges, labels);
    // }

    // template <typename EdgeData>
    // std::vector<EdgeData> createGraphData() {
    //     std::vector<EdgeData> edges{
    //         EdgeData(0, 1), EdgeData(0, 2), EdgeData(2, 2),
    //         EdgeData(0, 3), EdgeData(1, 4), EdgeData(2, 5),
    //         EdgeData(5, 7), EdgeData(3, 5), EdgeData(3, 6),
    //         EdgeData(4, 7), EdgeData(6, 4), EdgeData(6, 1),
    //         EdgeData(7, 6)};
    //     std::sort(edges.begin(), edges.end());
    //     return edges;
    // }

    // template <typename EdgeData>
    // std::vector<EdgeData> dag_graph() {
    //     std::vector<EdgeData> edges{
    //         EdgeData(0, 1), EdgeData(1, 3), EdgeData(1, 6),
    //         EdgeData(2, 0), EdgeData(3, 4), EdgeData(3, 5),
    //         EdgeData(4, 5), EdgeData(5, 6), EdgeData(2, 4),
    //         EdgeData(0, 3), EdgeData(2, 7)};
    //     std::sort(edges.begin(), edges.end());
    //     return edges;
    // }

    // std::vector<std::string> get_vertex_ids() {
    //     return std::vector<std::string>{"0", "1", "2", "3", "4", "5", "6", "7"};
    // }
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
    auto data = createGraphData<index_type>();
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
}

TEST(DFS, Positive) {
    using index_type = size_t;
    auto data = createGraphData<index_type>();
    auto edges = std::get<0>(data);
    auto labels = std::get<1>(data);
    using EdgeData = decltype(edges)::value_type;

    std::stringstream output;

    fmt::print("{}\n", output.str());
}


TEST(Serialization, Positive) {}

// TEST(TestSparseGraph, Positive) {
//     auto edges = createTestData<int>();
//     std::cout << "==== Edge information ====\n";
//     utils::print(edges);
//     graph::SparseGraph<int, int> g(edges, vertex_num(edges), true);

//     {
//         auto vertexes = g.vertexData();
//         decltype(vertexes) expectedVertexes{0, 3, 4, 6, 8, 9, 10, 12, 13};
//         EXPECT_EQ(expectedVertexes, vertexes);

//         auto edges = g.outEdgeData();
//         decltype(edges) expectedEdges{1, 2, 3, 4, 2, 5, 5, 6, 7, 7, 1, 4, 6};
//         EXPECT_EQ(expectedEdges, edges);
//     }

//     {
//         auto e = g.edges(0);
//         std::vector<int> results(std::get<0>(e), std::get<1>(e));
//         decltype(results) expectedResults{1, 2, 3};
//         EXPECT_EQ(results, expectedResults);
//     }

//     {
//         auto e = g.edges(6);
//         std::vector<int> results(std::get<0>(e), std::get<1>(e));
//         decltype(results) expectedResults{1, 4};
//         EXPECT_EQ(expectedResults, results);
//     }

//     {
//         auto e = g.edges(7);
//         std::vector<int> results(std::get<0>(e), std::get<1>(e));
//         decltype(results) expectedResults{6};
//         EXPECT_EQ(expectedResults, results);
//     }

//     // Check that we can generate a dot graph.
//     {
//         fmt::MemoryWriter writer;
//         graph::graph_info(g, writer);
//         fmt::print("{}\n", writer.str());
//         std::vector<std::string> v = get_vertex_ids();
//         std::string dotFile("test.dot");
//         graph::gendot<decltype(g), fmt::MemoryWriter>(g, v, dotFile);
//         // graph::viewdot(dotFile);
//     }
// }

// TEST(DFS, Positive) {
//     auto edges = createTestData<int>();
//     std::cout << "==== Edge information ====\n";
//     utils::print(edges);
//     graph::SparseGraph<int, int> g(edges, vertex_num(edges), true);

//     fmt::print("Visited vertexes\n");
//     {
//         using vertex_type = int;
//         using Container = std::vector<vertex_type>;
//         auto vertexes = g.vertexData();
//         auto edges = g.outEdgeData();

//         using DFSVisitor = graph::Visitor<decltype(g), Container>;
//         auto results = graph::dfs<decltype(g), DFSVisitor>(g, {0});

//         decltype(results) expectedResults{0, 3, 6, 4, 7, 1, 5, 2};
//         utils::print(results);
//         EXPECT_EQ(results, expectedResults);
//     }

//     // Generate a dot graph for a test graph.
//     // {
//     //     std::stringstream writer;
//     //     graph::graph_info(g, writer);
//     //     fmt::print("{}\n", writer.str());
//     //     std::vector<std::string> v = get_vertex_ids();
//     //     std::string dotFile("test.dot");
//     //     graph::gendot(g, v, dotFile);
//     //     graph::viewdot(dotFile);
//     // }
// }

// TEST(DFS2, Positive) {
//     auto edges = createTestData<int>();
//     std::cout << "==== Edge information ====\n";
//     utils::print(edges);
//     graph::SparseGraph<int, int> g(edges, vertex_num(edges), true);

//     fmt::print("Visited vertexes\n");
//     {
//         using vertex_type = int;
//         using Container = std::vector<vertex_type>;
//         auto vertexes = g.vertexData();
//         auto edges = g.outEdgeData();

//         using DFSVisitor = graph::Visitor<decltype(g), Container>;
//         DFSVisitor visitor(g);
//         auto results = graph::dfs<>(g, visitor, {0});

//         decltype(results) expectedResults{0, 3, 6, 4, 7, 1, 5, 2};
//         utils::print(results);
//         EXPECT_EQ(results, expectedResults);
//     }
// }

// TEST(BFS, Positive) {
//     auto edges = createTestData<int>();
//     std::cout << "==== Edge information ====\n";
//     utils::print(edges);
//     graph::SparseGraph<int, int> g(edges, vertex_num(edges), true);

//     fmt::print("Visited vertexes\n");
//     {
//         using vertex_type = int;
//         using Container = std::deque<vertex_type>;
//         auto vertexes = g.vertexData();
//         auto edges = g.outEdgeData();
//         using Visitor = graph::Visitor<decltype(g), Container>;
//         auto results = graph::bfs<decltype(g), Visitor>(g, {0});
//         decltype(results) expectedResults{0, 1, 2, 3, 4, 5, 6, 7};
//         utils::print(results);
//         EXPECT_EQ(results, expectedResults);
//     }

//     // Generate a dot graph for a test graph.
//     {
//         std::stringstream writer;
//         graph::graph_info(g, writer);
//         fmt::print("{}\n", writer.str());
//         std::vector<std::string> v = get_vertex_ids();
//         std::string dotFile("test.dot");
//         graph::gendot(g, v, dotFile);
//         // graph::viewdot(dotFile);
//     }
// }

// template <typename IArchive, typename OArchive> void test_cereal() {
//     auto edges = createTestData<int>();
//     std::cout << "==== Edge information ====\n";
//     utils::print(edges);
//     graph::SparseGraph<int, int> g(edges, vertex_num(edges), true);
//     auto v = g.vertexData();
//     auto e = g.outEdgeData();

//     fmt::MemoryWriter writer;
//     graph::graph_info(g, writer);
//     fmt::print("{}\n", writer.str());
//     std::vector<std::string> vids = get_vertex_ids();

//     // Serialize the data to output string stream
//     std::ostringstream os;

//     {
//         OArchive output(os);
//         utils::save_sparse_graph(output, g, vids);
//     }

//     // Deserialize the data from a string stream
//     decltype(g)::vertex_container v_read;
//     decltype(g)::edge_container e_read;
//     std::vector<std::string> vids_read;

//     std::istringstream is(os.str());
//     IArchive input(is);
//     input(vids_read, v_read, e_read);

//     utils::print(vids_read);
//     utils::print(v_read);
//     utils::print(e_read);

//     EXPECT_EQ(v, v_read);
//     EXPECT_EQ(e, e_read);
//     EXPECT_EQ(vids, vids_read);
// }

// TEST(SerializationUsingCereal, Positive) {
//     using OArchive = cereal::BinaryOutputArchive;
//     using IArchive = cereal::BinaryInputArchive;
//     test_cereal<IArchive, OArchive>();
//     test_cereal<cereal::JSONInputArchive, cereal::JSONOutputArchive>();
// }

// TEST(Sorted_List, Positive) {
//     auto edges = dag_graph<int>();
//     std::cout << "==== Edge information ====\n";
//     utils::print(edges);
//     graph::SparseGraph<int, int> g(edges, vertex_num(edges), true);

//     // Generate a dot graph for a test graph.
//     {
//         std::stringstream writer;
//         graph::graph_info(g, writer);
//         fmt::print("{}\n", writer.str());
//         std::vector<std::string> v{"0", "1", "2", "3", "4", "5", "6"};
//         std::string dotFile("test_dag.dot");
//         graph::gendot(g, v, dotFile);
//         // graph::viewdot(dotFile);
//     }

//     fmt::print("Visited vertexes\n");
//     {
//         using vertex_type = int;
//         using Container = std::vector<vertex_type>;
//         auto vertexes = g.vertexData();
//         auto edges = g.outEdgeData();
//         using DFSVisitor = graph::Visitor<decltype(g), Container>;
//         auto results = graph::dfs<decltype(g), DFSVisitor>(g, {0});
//         decltype(results) expectedResults{0, 3, 5, 6, 4, 1};
//         utils::print(results);
//         EXPECT_EQ(results, expectedResults);
//     }
// }

// TEST(Connected_components, Positive) {
//     auto edges = dag_graph<int>();
//     std::cout << "==== Edge information ====\n";
//     utils::print(edges);
//     graph::SparseGraph<int, int> g(edges, vertex_num(edges), true);
//     using vertex_type = int;
//     using Container = std::vector<vertex_type>;
//     using DFSVisitor = graph::Visitor<decltype(g), Container>;
//     DFSVisitor visitor(g);
//     auto results = graph::connected_components(g, visitor);
//     decltype(results) expected_results = {{0, 3, 5, 6, 4, 1}, {2, 7}};
//     EXPECT_EQ(expected_results, results);

//     for (auto item : results) {
//         fmt::print("== A component ==\n");
//         utils::print(item);
//     }

//     // Generate a dot graph for a test graph.
//     {
//         std::stringstream writer;
//         graph::graph_info(g, writer);
//         fmt::print("{}\n", writer.str());
//         std::string dotFile("test_dag.dot");
//         auto v = get_vertex_ids();
//         graph::gendot(g, v, dotFile);
//         // graph::viewdot(dotFile);
//     }
// }
