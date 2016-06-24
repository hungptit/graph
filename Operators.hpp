#ifndef Operators_hpp
#define Operators_hpp

#include "DataStructures.hpp"

namespace graph {
    template <typename index_type>
    bool operator==(const BasicEdgeData<index_type> &lhs,
                    const BasicEdgeData<index_type> &rhs) {
        return lhs.SrcID == rhs.SrcID && lhs.DstID == rhs.DstID;
    }

    template <typename index_type, typename EdgeData> struct Greater;

    template <typename index_type> struct Greater<index_type, BasicEdgeData<index_type>> {
        bool operator()(BasicEdgeData<index_type> a, BasicEdgeData<index_type> b) {
            return (a.SrcId > b.SrcId) || ((a.SrcId == b.SrcId) && (a.DstId > b.DstId));
        }
    };

    // template <typename BasicEdgeData> struct Greater {
    //     bool operator()(BasicEdgeData a, BasicEdgeData b) {
    //         return (a.SrcId > b.SrcId) || ((a.SrcId == b.SrcId) && (a.DstId > b.DstId));
    //     }
    // };

    // template <typename BasicEdgeData> struct Less {
    //     bool operator()(BasicEdgeData a, BasicEdgeData b) {
    //         return (a.SrcId < b.SrcId) || ((a.SrcId == b.SrcId) && (a.DstId < b.DstId));
    //     }
    // };
}

#endif
