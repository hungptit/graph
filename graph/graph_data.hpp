#pragma once

#include <vector>

// TODO: Review all structs and make sure that they are compliance to the guide line.

namespace graph {
    // A convenient struct which hold both graph and vertexes information.
    template <typename T1, typename T2> struct GraphData {
        using graph_type = T1;
        using vertex_container = T2;

        explicit GraphData() noexcept = default;

        explicit GraphData(const T1 &g, const T2 &v) : graph(g), vertexes(v) {}

        explicit GraphData(T1 &&g, T2 &&v) noexcept
            : graph(std::forward<T1>(g)), vertexes(std::forward<T2>(v)){};

        graph_type graph;
        vertex_container vertexes;

        template <typename Archive> void serialize(Archive &ar) { ar(graph, vertexes); }
    };
} // namespace graph
