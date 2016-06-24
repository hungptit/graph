#ifndef Utils_hpp
#defien Utils_hpp

#include "DataStructures.hpp"
#include "Operators.hpp"

namespace graph {
    // Utilty functions    
    template <typename Graph, typename Writer>
    void graph_info(Graph &g, Writer &writer) {
        auto vertexes = g.vertexData();
        auto edges = g.outEdgeData();
        auto N = vertexes.size() - 1;
        for (std::size_t currentCol = 0; currentCol < N; ++currentCol) {
            auto begin = vertexes[currentCol];
            auto end = vertexes[currentCol + 1];
            writer << "Vetex[" << currentCol << "]: Edges = {";

            if (begin != end) {
                writer << edges[begin];
                begin++;
            }

            for (auto idx = begin; idx < end; ++idx) {
                writer << "," << edges[idx];
            }
            writer << "}\n";
        }
    }

    /// Write input graph information to a dot file.
    template <typename Graph, typename Writer = std::stringstream>
    void gendot(Graph &g, std::vector<std::string> &v,
                const std::string &dotfile) {
        Writer writer;
        writer << (g.isDirected() ? ("digraph") : ("graph"))
               << (" G {\n"); // Header

        // Write vertex's information
        int i = 0;
        writer << "\t" << i << "[peripheries=2, label=\"" << v[i]
               << "\",style=bold,shape=box]\n";
        for (auto const &val : v) {
            writer << "\t" << i << "[label=\"" << val << "\"]\n";
            i++;
        }

        auto vertexes = g.vertexData();
        auto edges = g.outEdgeData();

        // Write edge's' information
        const std::string direction = g.isDirected() ? "->" : "--";
        std::size_t N = vertexes.size() - 1;
        for (std::size_t vid = 0; vid < N; ++vid) {
            auto begin = vertexes[vid];
            auto end = vertexes[vid + 1];
            for (auto it = begin; it != end; ++it) {
                writer << "\t" << vid << direction << edges[it] << "\n";
            }
        }

        writer << "}\n"; // Footer

        // Write data to a dot file
        std::ofstream myfile;
        myfile.open(dotfile);
        myfile << writer.str();
    }

    /// View a dot file using dot command.
    void viewdot(const std::string &dotFile) {
        const std::string cmd = "dot -Txlib " + dotFile;
        std::system(cmd.c_str());
    }

    template <typename Graph, typename Writer = std::stringstream>
    void tree_info(const Graph &g, std::vector<std::string> &vids) {
        graph_info(g);
        Writer writer;
        size_t counter = 0;
        for (auto item : vids) {
            writer << "vid[" << counter << "] = " << item << "\n";
            counter++;
        }
        std::cout << writer.str();
    }
}

#endif
