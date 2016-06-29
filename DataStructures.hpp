#ifndef DataStructures_hpp_
#define DataStructures_hpp_

#include <string>
#include <vector>

#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/bitset.hpp"
#include "cereal/types/chrono.hpp"
#include "cereal/types/complex.hpp"
#include "cereal/types/deque.hpp"
#include "cereal/types/forward_list.hpp"
#include "cereal/types/queue.hpp"
#include "cereal/types/set.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal/types/utility.hpp"
#include "cereal/types/valarray.hpp"
#include "cereal/types/vector.hpp"

#include <algorithm>

namespace graph {
    template <typename index_type> struct BasicEdgeData {
        index_type SrcId;
        index_type DstId;

        BasicEdgeData() {}

        BasicEdgeData(const index_type srcId, const index_type dstId)
            : SrcId(srcId), DstId(dstId) {}

        BasicEdgeData(const BasicEdgeData &data) : SrcId(data.SrcId), DstId(data.DstId){};

        BasicEdgeData(BasicEdgeData &&data) : SrcId(data.SrcId), DstId(data.DstId){};

        BasicEdgeData &operator=(BasicEdgeData rhs) {
            std::swap(SrcId, rhs.SrcId);
            std::swap(DstId, rhs.DstId);
            return *this;
        }

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("srcid", SrcId), cereal::make_nvp("dstid", DstId));
        }
    };

    template <typename index_type, typename weight_type> struct WeightedEdgeData {
        index_type SrcId;
        index_type DstId;
        weight_type Weight;

        WeightedEdgeData(const index_type srcId, const index_type dstId,
                         const weight_type weight)
            : SrcId(srcId), DstId(dstId), Weight(weight) {}

        WeightedEdgeData(const WeightedEdgeData &data)
            : SrcId(data.SrcId), DstId(data.DstId), Weight(data.Weight){};

        WeightedEdgeData(WeightedEdgeData &&data)
            : SrcId(data.SrcId), DstId(data.DstId), Weight(data.Weight){};

        WeightedEdgeData &operator=(WeightedEdgeData rhs) {
            std::swap(SrcId, rhs.SrcId);
            std::swap(DstId, rhs.DstId);
            std::swap(Weight, rhs.Weight);
            return *this;
        }

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("srcid", SrcId), cereal::make_nvp("dstid", DstId),
               cereal::make_nvp("weight", Weight));
        }
    };

    /**
     * Define some operators for EdgeData claases.
     *
     */

    template <typename index_type>
    bool operator==(const BasicEdgeData<index_type> &lhs,
                    const BasicEdgeData<index_type> &rhs) {
        return lhs.SrcId == rhs.SrcId && lhs.DstId == rhs.DstId;
    }

    template <typename index_type, typename edge_type>
    bool operator==(const WeightedEdgeData<index_type, edge_type> &lhs,
                    const WeightedEdgeData<index_type, edge_type> &rhs) {
        return lhs.SrcId == rhs.SrcId && lhs.DstId == rhs.DstId && lhs.Weight == rhs.Weight;
    }

    // Greater comparators
    template <typename index_type, typename EdgeData> struct Greater;

    template <typename index_type> struct Greater<index_type, BasicEdgeData<index_type>> {
        bool operator()(BasicEdgeData<index_type> a, BasicEdgeData<index_type> b) {
            return (a.SrcId > b.SrcId) || ((a.SrcId == b.SrcId) && (a.DstId > b.DstId));
        }
    };

    // Less comparators
    template <typename index_type, typename EdgeData> struct Less;
    template <typename index_type> struct Less<index_type, BasicEdgeData<index_type>> {
        bool operator()(BasicEdgeData<index_type> a, BasicEdgeData<index_type> b) {
            return (a.SrcId < b.SrcId) || ((a.SrcId == b.SrcId) && (a.DstId < b.DstId));
        }
    };

    // Simple implementation for a graph using CSR format.
    template <typename itype, typename etype> struct SparseGraph {
        using index_type = itype;
        using edge_type = etype;
        using edge_container = std::vector<edge_type>;
        using vertex_container = std::vector<index_type>;
        using edge_iterator = typename edge_container::const_iterator;

        explicit SparseGraph() : Vertexes(), Edges(), IsDirected() {}

        explicit SparseGraph(std::vector<edge_type> &data, const std::size_t N,
                             const bool isDirected)
            : IsDirected(isDirected) {
            build(data, N);
        }

        explicit SparseGraph(vertex_container &&v, edge_container &&e, bool isDirected) noexcept
            : Vertexes(std::move(v)),
              Edges(std::move(e)),
              IsDirected(isDirected) {}

        void build(std::vector<edge_type> &edges, const std::size_t N) {
            /// This function assume that edges vector is sorted.
            std::is_sorted(edges.begin(), edges.end(), graph::Less<index_type, edge_type>());
            Vertexes.assign(N + 1, 0);
            Edges.reserve(edges.size());
            for (auto const &val : edges) {
                Vertexes[val.SrcId + 1]++;
                Edges.emplace_back(val);
            }

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
        return lhs.Vertexes == rhs.Vertexes && lhs.Edges == rhs.Edges &&
               lhs.IsDirected == rhs.IsDirected;
    }
}

#endif
