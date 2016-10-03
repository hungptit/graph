#include "gtest/gtest.h"
#include <iostream>
#include <tuple>
#include <vector>

#include "fmt/format.h"

#include "graph/SparseGraph.hpp"
#include "Data.hpp"

#include "celero/Celero.h"

constexpr int NumberOfSamples = 200000;
constexpr int NumberOfIterations = 10;

const auto graph_int = simpleDirectedGraph<int>();
const auto graph_unsigned_int = simpleDirectedGraph<unsigned int>();
const auto graph_size_t = simpleDirectedGraph<size_t>();

namespace {
    template<typename index_type> auto dfs_preordering() {
        auto data = simpleDirectedGraph<index_type>();
        auto edges = std::get<0>(data);
        auto labels = std::get<1>(data);
        auto N = std::get<2>(data);
        std::stringstream output;
        graph::SparseGraph<index_type, typename decltype(edges)::value_type> g(edges, N, true);
        index_type rootVid = 0;
        return graph::dfs_preordering<std::vector<index_type>>(g, {rootVid});
    }

    template <typename index_type> auto dfs_postordering() {
        auto data = simpleDirectedGraph<index_type>();
        auto edges = std::get<0>(data);
        auto labels = std::get<1>(data);
        auto N = std::get<2>(data);
        std::stringstream output;

        graph::SparseGraph<index_type, typename decltype(edges)::value_type> g(edges, N, true);
        index_type rootVid = 0;
        return graph::dfs_postordering<std::vector<index_type>>(g, {rootVid});
    }
}

CELERO_MAIN

BASELINE(dfs_preordering, int, NumberOfSamples, NumberOfIterations) {
    celero::DoNotOptimizeAway(dfs_preordering<int>());
}

BENCHMARK(dfs_preordering, unsigned_int, NumberOfSamples, NumberOfIterations) {
    celero::DoNotOptimizeAway(dfs_preordering<unsigned int>());
}

BENCHMARK(dfs_preordering, size_t, NumberOfSamples, NumberOfIterations) {
    celero::DoNotOptimizeAway(dfs_preordering<size_t>());
}

BASELINE(dfs_postordering, int, NumberOfSamples, NumberOfIterations) {
    celero::DoNotOptimizeAway(dfs_postordering<int>());
}

BENCHMARK(dfs_postordering, unsigned_int, NumberOfSamples, NumberOfIterations) {
    celero::DoNotOptimizeAway(dfs_postordering<unsigned int>());
}

BENCHMARK(dfs_postordering, size_t, NumberOfSamples, NumberOfIterations) {
    celero::DoNotOptimizeAway(dfs_postordering<size_t>());
}
