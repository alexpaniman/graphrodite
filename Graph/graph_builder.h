#pragma once
#include "graph.h"
#include <map>

namespace graph {
    template <typename DataT> 
    class GraphBuilder {
    
    public:
        GraphBuilder(Graph<DataT>& graph_): graph(graph_) {}

        Graph<DataT>& get_graph() {
            return graph;
        }
        
        bool add_vertex(DataT data) {
            if (builded_nodes.find(data) != builded_nodes.end()) {
                NodeId new_node_id = graph.create_node(data);
                builded_nodes[data] = new_node_id;
                return true;
            }

            return false;
        }

        bool add_edge(DataT from, DataT to) {
            NodeId from_id = builded_nodes.find(from);
            NodeId to_id = builded_nodes.find(to);

            if (from_id == -1 or to_id == -1)
                return false;
            
            graph.nodes[from_id].successors.append(to_id);
        }
        
        NodeId get_vertex(DataT vertex_data) const {
            auto& was_found = builded_nodes.find(vertex_data);

            if (was_found != builded_nodes.end())
                return was_found.second;

            return -1;
        }

        virtual ~GraphBuilder() = default;
    private:
        Graph<DataT>& graph;
        std::map<DataT, NodeId> builded_nodes;
    };
}