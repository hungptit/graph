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

    // Simple implementation for a graph using CSR format.
    template <typename itype, typename etype> struct SparseGraph {
        using index_type = itype;
        using edge_type = etype;
        using edge_container = std::vector<edge_type>;
        using vertex_container = std::vector<index_type>;
        using edge_iterator = typename edge_container::const_iterator;

        explicit SparseGraph(const bool isDirected) : IsDirected(isDirected) {}

        explicit SparseGraph(std::vector<std::tuple<index_type, index_type>> &data,
                             const std::size_t N, const bool isDirected)
            : IsDirected(isDirected) {
            build(data, N);
        }

        explicit SparseGraph(vertex_container &&v, edge_container &&e, bool isDirected) noexcept
            : Vertexes(std::move(v)),
              Edges(std::move(e)),
              IsDirected(isDirected) {}

        /// This function assume that input data is sorted by vertex ID.
        void build(std::vector<etype> &data, const std::size_t N) {
            Vertexes.assign(N + 1, 0);
            Edges.assign(data.size(), 0);
            index_type currentRow = 0;
            for (auto const &val : data) {
                Vertexes[val.SrcId + 1]++;
                Edges[currentRow++] = val;
            }

            for (std::size_t currentCol = 0; currentCol < N; ++currentCol) {
                Vertexes[currentCol + 1] += Vertexes[currentCol];
            }
        }

        bool isDirected() { return IsDirected; };

        edge_type getEdge(const index_type eid) { return Edges[eid]; }

        std::array<index_type, 2> edges(const index_type vid) const {
            return {Vertexes[vid], Vertexes[vid + 1]};
        }

        std::tuple<edge_iterator, edge_iterator> edge_iterators(const index_type vid) const {
            return std::make_tuple(std::advance(Edges.begin(), Vertexes[vid]),
                                   std::advance(Edges.begin(), Vertexes[vid + 1]));
        }

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("IsDirected", IsDirected),
               cereal::make_nvp("Vertexes", Vertexes), cereal::make_nvp("Edges", Edges));
        }

        // Graph data
        std::vector<index_type> Vertexes;
        std::vector<edge_type> Edges;
        const bool IsDirected;
    };
}

#endif
