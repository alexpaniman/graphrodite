#pragma once
#include <llvm/ADT/SmallVector.h>
#include <vector>
#include "graph_iters.h"

//--------------------------------------------------------------------------------

namespace graph {
    using NodeId = int;

    namespace realization {
        template <typename DataT>
        struct RawNode {
            RawNode(DataT data_): data(data_) {}

            DataT data;
            llvm::SmallVector<NodeId, 8> successors;
        };
    };

    template <typename DataT>
    class Graph {
    
    public:
        NodeId create_node(DataT data) { 
            nodes.emplace(nodes.end(), realization::RawNode(data)); 
            return nodes.size();
        }
        
        traverse_range<DataT> traverse() { 
            return traverse_range<DataT>(*this);
        }

        successors_range<DataT> get_successors(realization::RawNode<DataT> node) {
            auto search_result = std::find(nodes.begin(), nodes.end(), node);
            
            if (search_result != nodes.end())
                return successors_range<DataT>(*this, *search_result - nodes.begin());
            
            return successors_range<DataT>(*this, nodes.end());
        }

        std::vector<realization::RawNode<DataT>> nodes;
    };

//--------------------------------------------------------------------------------

    template <typename DataT>
    class Node {
    
    public:
        Node(Graph<DataT>& graph_, NodeId node_id_):
            graph(grpah_),
            node_id(node_id_)    
        { }

        operator realization::RawNode() const {
            return graph.nodes[node_id];
        }

        successors_range get_successors() {
            return graph.get_successors(graph.nodes[node_id]);
        }

        DataT& get_data() {
            return graph.nodes[node_id].data
        }

        private:
            Graph<DataT>& graph;
            NodeId node_id;
    };

} // namespace graph