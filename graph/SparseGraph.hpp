/**
 * @file   SparseGraph.hpp
 * @author Hung Dang <hungptit@gmail.com>
 * @date   Fri Mar  4 18:46:58 2016
 *
 * @brief  This file has a simple implementation of a directed sparse graph.
 *
 */

#pragma once

#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

// fmt
#include "fmt/format.h"

// Cereal
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/deque.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/set.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal/types/vector.hpp"

// graph
#include "BFS.hpp"
#include "DFS.hpp"
#include "DataStructures.hpp"
#include "TopologicalSortedList.hpp"
#include "Utils.hpp"
