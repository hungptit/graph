#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace graph {
    enum class NodeStatus { UNDISCOVERED, VISITED, DISCOVERED, PROCESSED };
    enum class NodeColors { BLACK, WHITE };

    /**
     * BasicEdgeData
     *
     */
    template <typename itype> struct BasicEdgeData {
        using index_type = itype;
        index_type SrcId;
        index_type DstId;

        BasicEdgeData() noexcept {}

        BasicEdgeData(const index_type srcId, const index_type dstId) noexcept
            : SrcId(srcId), DstId(dstId) {}

        BasicEdgeData(const BasicEdgeData &data) noexcept = default;

        BasicEdgeData(BasicEdgeData &&data) noexcept = default;

        BasicEdgeData &operator=(const BasicEdgeData &) noexcept = default;

        template <typename Archive> void serialize(Archive &ar) { ar(SrcId, DstId); }
    };

    template <typename index_type>
    bool operator==(const BasicEdgeData<index_type> &lhs,
                    const BasicEdgeData<index_type> &rhs) {
        return std::tie(lhs.SrcId, lhs.DstId) == std::tie(rhs.SrcId, rhs.DstId);
    }

    template <typename index_type>
    bool operator<(const BasicEdgeData<index_type> &lhs, const BasicEdgeData<index_type> &rhs) {
        return std::tie(lhs.SrcId, lhs.DstId) < std::tie(rhs.SrcId, rhs.DstId);
    }

    template <typename index_type>
    bool operator>(const BasicEdgeData<index_type> &lhs, const BasicEdgeData<index_type> &rhs) {
        return std::tie(lhs.SrcId, lhs.DstId) > std::tie(rhs.SrcId, rhs.DstId);
    }

    /*
     * WeightedEdgeData
     *
     */
    template <typename index_type, typename weight_type> struct WeightedEdgeData {
        index_type SrcId;
        index_type DstId;
        weight_type Weight;

        WeightedEdgeData(const index_type srcId, const index_type dstId,
                         const weight_type weight) noexcept
            : SrcId(srcId), DstId(dstId), Weight(weight) {}

        WeightedEdgeData(const WeightedEdgeData &data) noexcept = default;

        WeightedEdgeData(WeightedEdgeData &&data) noexcept = default;

        WeightedEdgeData &operator=(const WeightedEdgeData &rhs) = default;

        template <typename Archive> void serialize(Archive &ar) { ar(SrcId, DstId, Weight); }
    };

    template <typename index_type, typename weight_type>
    bool operator==(const WeightedEdgeData<index_type, weight_type> &lhs,
                    const WeightedEdgeData<index_type, weight_type> &rhs) {
        return std::tie(lhs.SrcId, lhs.DstId, lhs.Weight) ==
               std::tie(rhs.SrcId, rhs.DstId, rhs.Weight);
    }

    template <typename index_type, typename weight_type>
    bool operator<(const WeightedEdgeData<index_type, weight_type> &lhs,
                   const WeightedEdgeData<index_type, weight_type> &rhs) {
        return std::tie(lhs.SrcId, lhs.DstId, lhs.Weight) <
               std::tie(rhs.SrcId, rhs.DstId, rhs.Weight);
    }

    template <typename index_type, typename weight_type>
    bool operator>(const WeightedEdgeData<index_type, weight_type> &lhs,
                   const WeightedEdgeData<index_type, weight_type> &rhs) {
        return std::tie(lhs.SrcId, lhs.DstId, lhs.Weight) >
               std::tie(rhs.SrcId, rhs.DstId, rhs.Weight);
    }

    // Simple implementation for a graph using CSR format.
    template <typename itype, typename etype> struct SparseGraph {
        using index_type = itype;
        using edge_type = etype;
        using edge_container = std::vector<edge_type>;
        using vertex_container = std::vector<index_type>;
        using edge_iterator = typename edge_container::const_iterator;

        SparseGraph() noexcept : vertexes_(), edges_(), directed() {}

        template <typename T>
        SparseGraph(T &&data, const std::size_t N, const bool isDirected) noexcept
            : vertexes_(), edges_(), directed(isDirected) {
            build(std::forward<T>(data), N);
        }

        SparseGraph(vertex_container &&v, edge_container &&e, bool isDirected) noexcept
            : vertexes_(std::move(v)), edges_(std::move(e)), directed(isDirected) {}

        template <typename Container>
        void build(Container &&edges, const std::size_t N, bool isDirected = true) {
            /// This function assume that edges vector is sorted.
            assert(std::is_sorted(edges.begin(), edges.end()));

            directed = isDirected;
            edges_ = std::forward<Container>(edges);

            // Update the vertex information.
            vertexes_.assign(N + 1, 0);
            std::for_each(edges_.cbegin(), edges_.cend(), [&](auto const &anEdge) {
                // Make sure that vertex and edge information are synced.
                assert(static_cast<index_type>(N) > anEdge.SrcId);
                vertexes_[anEdge.SrcId + 1]++;
            });

            // Update the begin and end location for each column.
            for (std::size_t currentCol = 0; currentCol < N; ++currentCol) {
                vertexes_[currentCol + 1] += vertexes_[currentCol];
            }
        }

        bool isdirected() const { return directed; };
        const index_type begin(const index_type vid) const { return vertexes_[vid]; }
        const index_type end(const index_type vid) const { return vertexes_[vid + 1]; }
        const edge_type edge(const index_type eid) const { return edges_[eid]; }
        size_t number_of_vertexes() const { return vertexes_.size() - 1; }
        size_t number_of_edges() const { return edges_.size(); }
        template <typename Archive> void serialize(Archive &ar) {
            ar(directed, vertexes_, edges_);
        }

        template <typename index_type, typename edge_type>
        friend bool operator==(const SparseGraph<index_type, edge_type> &lhs,
                               const SparseGraph<index_type, edge_type> &rhs);

      private:
        std::vector<index_type> vertexes_;
        std::vector<edge_type> edges_;
        bool directed;
    };

    template <typename index_type, typename edge_type>
    bool operator==(const SparseGraph<index_type, edge_type> &lhs,
                    const SparseGraph<index_type, edge_type> &rhs) {
        return std::tie(lhs.directed, lhs.vertexes_, lhs.edges_) ==
               std::tie(rhs.directed, rhs.vertexes_, rhs.edges_);
    }
} // namespace graph

// Customized hash function for different edge type.
namespace std {
    template <typename index_type> struct hash<graph::BasicEdgeData<index_type>> {
        using result_type = std::size_t;
        result_type operator()(graph::BasicEdgeData<index_type> const &value) const {
            result_type const h1(std::hash<index_type>()(value.SrcId));
            result_type const h2(std::hash<index_type>()(value.DstId));
            return h1 ^ (h2 << 4);
        }
    };

    template <typename index_type, typename weight_type>
    struct hash<graph::WeightedEdgeData<index_type, weight_type>> {
        using result_type = std::size_t;
        result_type
        operator()(graph::WeightedEdgeData<index_type, weight_type> const &value) const {
            result_type const h1(std::hash<index_type>()(value.SrcId));
            result_type const h2(std::hash<index_type>()(value.DstId));
            result_type const h3(std::hash<weight_type>()(value.Weight));
            return h1 ^ (h2 << 4) ^ (h3 << 8);
        }
    };
} // namespace std
