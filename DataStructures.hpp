#ifndef Graph_DataStructures_hpp_
#define Graph_DataStructures_hpp_

#include <string>
#include <vector>
#include <algorithm>

#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

namespace graph {
    enum NodeStatus { UNDISCOVERED, VISITED, DISCOVERED, PROCESSED };
    enum NodeColors { BLACK, WHITE };

    /**
     * BasicEdgeData
     *
     */
    template <typename itype> struct BasicEdgeData {
        using index_type = itype;
        index_type SrcId;
        index_type DstId;

        BasicEdgeData() {}

        BasicEdgeData(const index_type srcId, const index_type dstId) noexcept
            : SrcId(srcId), DstId(dstId)  {}

        BasicEdgeData(const BasicEdgeData &data) noexcept : SrcId(data.SrcId), DstId(data.DstId){};

        BasicEdgeData(BasicEdgeData &&data) : SrcId(data.SrcId), DstId(data.DstId){};

        BasicEdgeData &operator=(const BasicEdgeData &) = default;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("srcid", SrcId), cereal::make_nvp("dstid", DstId));
        }
    };

    template <typename index_type>
    bool operator==(const BasicEdgeData<index_type> &lhs,
                    const BasicEdgeData<index_type> &rhs) {
        return lhs.SrcId == rhs.SrcId && lhs.DstId == rhs.DstId;
    }

    template <typename index_type>
    bool operator<(const BasicEdgeData<index_type> &lhs, const BasicEdgeData<index_type> &rhs) {
        return (lhs.SrcId < rhs.SrcId) || ((lhs.SrcId == rhs.SrcId) && (lhs.DstId < rhs.DstId));
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

        WeightedEdgeData(const WeightedEdgeData &data) noexcept
            : SrcId(data.SrcId), DstId(data.DstId), Weight(data.Weight){};

        WeightedEdgeData(WeightedEdgeData &&data) noexcept
            : SrcId(data.SrcId), DstId(data.DstId), Weight(data.Weight){};

        WeightedEdgeData &operator=(const WeightedEdgeData &rhs) = default;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("srcid", SrcId), cereal::make_nvp("dstid", DstId),
               cereal::make_nvp("weight", Weight));
        }
    };

    template <typename index_type, typename edge_type>
    bool operator==(const WeightedEdgeData<index_type, edge_type> &lhs,
                    const WeightedEdgeData<index_type, edge_type> &rhs) {
        return lhs.SrcId == rhs.SrcId && lhs.DstId == rhs.DstId && lhs.Weight == rhs.Weight;
    }

    template <typename index_type, typename edge_type>
    bool operator<(const WeightedEdgeData<index_type, edge_type> &lhs,
                   const WeightedEdgeData<index_type, edge_type> &rhs) {
        return (lhs.SrcId < rhs.SrcId) ||
               ((lhs.SrcId == rhs.SrcId) && (lhs.DstId < rhs.DstId)) ||
               ((lhs.SrcId == rhs.SrcId) && (lhs.DstId == rhs.DstId) &&
                (lhs.Weight < rhs.Weight));
    }

    // Simple implementation for a graph using CSR format.
    template <typename itype, typename etype> struct SparseGraph {
        using index_type = itype;
        using edge_type = etype;
        using edge_container = std::vector<edge_type>;
        using vertex_container = std::vector<index_type>;
        using edge_iterator = typename edge_container::const_iterator;

        explicit SparseGraph() : Vertexes(), Edges(), IsDirected() {}

        template <typename T>
        explicit SparseGraph(T &&data, const std::size_t N, const bool isDirected) noexcept
            : Vertexes(), Edges(), IsDirected(isDirected) {
          build(data, N);
        }

        explicit SparseGraph(vertex_container &&v, edge_container &&e, bool isDirected) noexcept
            : Vertexes(std::move(v)), Edges(std::move(e)), IsDirected(isDirected) {}

        template <typename Container> void build(Container &&edges, const std::size_t N) {
            /// This function assume that edges vector is sorted.
            assert(std::is_sorted(edges.begin(), edges.end()));

            Edges = std::move(edges);

            // Update the vertex information.
            Vertexes.assign(N + 1, 0);
            std::for_each(Edges.cbegin(), Edges.cend(),
                          [&](auto const &anEdge) { Vertexes[anEdge.SrcId + 1]++; });

            // Update the begin and end location for each column.
            for (std::size_t currentCol = 0; currentCol < N; ++currentCol) {
                Vertexes[currentCol + 1] += Vertexes[currentCol];
            }
        }

        bool isDirected() const { return IsDirected; };
        const index_type begin(const index_type vid) const { return Vertexes[vid]; }
        const index_type end(const index_type vid) const { return Vertexes[vid + 1]; }
        const edge_type edge(const index_type eid) const { return Edges[eid]; }
        size_t numberOfVertexes() const { return Vertexes.size() - 1; }

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("IsDirected", IsDirected),
               cereal::make_nvp("Vertexes", Vertexes), cereal::make_nvp("Edges", Edges));
        }

        template <typename index_type, typename edge_type>
        friend bool operator==(const SparseGraph<index_type, edge_type> &lhs,
                               const SparseGraph<index_type, edge_type> &rhs);

      private:
        std::vector<index_type> Vertexes;
        std::vector<edge_type> Edges;
        bool IsDirected;
    };

    template <typename index_type, typename edge_type>
    bool operator==(const SparseGraph<index_type, edge_type> &lhs,
                    const SparseGraph<index_type, edge_type> &rhs) {
        return lhs.IsDirected == rhs.IsDirected && lhs.Vertexes == rhs.Vertexes &&
          lhs.Edges == rhs.Edges;
    }
}

namespace std {
    template <typename index_type> struct hash<graph::BasicEdgeData<index_type>> {
        using result_type = std::size_t;
        result_type operator()(graph::BasicEdgeData<index_type> const value) const {
            result_type const h1(std::hash<index_type>()(value.SrcId));
            result_type const h2(std::hash<index_type>()(value.DstId));
            return h1 ^ (h2 << 4);
        }
    };

    template <typename index_type, typename weight_type>
    struct hash<graph::WeightedEdgeData<index_type, weight_type>> {
        using result_type = std::size_t;
        result_type
        operator()(graph::WeightedEdgeData<index_type, weight_type> const value) const {
            result_type const h1(std::hash<index_type>()(value.SrcId));
            result_type const h2(std::hash<index_type>()(value.DstId));
            result_type const h3(std::hash<weight_type>()(value.Weight));
            return h1 ^ (h2 << 4) ^ (h3 << 8);
        }
    };
}

#endif
