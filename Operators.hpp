#ifndef Operators_hpp
#define Operators_hpp

#include "DataStructures.hpp"

namespace graph {
    bool operator==(const EdgeData &lhs, const EdgeData &rhs) {
        return lhs.SrcID == rhs.SrcID && lhs.DstID == rhs.DstID;
    }

    template<typename T> struct Greater;
    template<typename T> struct Less;

    template <> struct Greater<EdgeData> {
        bool operator()(EdgeData a, EdgeData b) {
            return (a.SrcID > b.SrcID) || ((a.SrcID == b.SrcID) && (a.DstID > b.DstID));
        }
    };

    template <> struct Less<EdgeData> {
        bool operator()(EdgeData a, EdgeData b) {
            return (a.SrcID < b.SrcID) || ((a.SrcID == b.SrcID) && (a.DstID < b.DstID));
        }
    };
}

#endif
