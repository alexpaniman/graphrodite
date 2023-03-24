#pragma once
#include "graph.h"
#include <map>
#include <optional>

namespace graph_lib {
    template <typename DataT> 
    class GraphBuilder {
    
    public:
        Graph<DataT> get_graph() { return graph; }
        
        bool add_vertex(DataT data) {
            if (!builded_nodes.contains(data)) {
                builded_nodes[data] = graph.create_node(data);
                return true;
            }

            return false;
        }

        bool add_edge(DataT from, DataT to) {
            auto from_id = get_vertex(from);
            auto to_id = get_vertex(to);

            if (!from_id or !to_id)
                return false;
            
            auto& successors = graph.storage.nodes[*from_id].successors;
            // TODO:                 ^~~~~~~~~~~~~~~~~~~~~~~ should replace by accessor function in Graph

            successors.emplace_back(*to_id);
            return true;
        }
        
        std::optional<NodeId> get_vertex(DataT vertex_data) {
            auto was_found = builded_nodes.find(vertex_data);

            if (was_found != builded_nodes.end())
                return was_found->second;

            return std::nullopt;
        }
        
    private:
        Graph<DataT> graph;
        std::map<DataT, NodeId> builded_nodes;
    };
}
