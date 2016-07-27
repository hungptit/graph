#ifndef Graph_Utils_hpp
#define Graph_Utils_hpp

#include "DataStructures.hpp"

namespace graph {
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

        // Write edge's' information
        const std::string direction = g.isDirected() ? "->" : "--";
        std::size_t N = g.numberOfVertexes();
        for (std::size_t vid = 0; vid < N; ++vid) {
            auto begin = g.begin(vid);
            auto end = g.end(vid);
            for (auto it = begin; it != end; ++it) {
                writer << "\t" << vid << direction << g.edge(it).DstId << "\n";
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
}

#endif
