
#pragma once
#include "graph.h"

namespace graph {
    template <typename DataT>
    class ProxyNode {
    
    public:
        ProxyNode(Graph<DataT>& graph_, NodeId node_id_):
            graph(grpah_),
            node_id(node_id_)    
        { }

        operator Node() const {
            return graph.nodes[node_id];
        }

        successors_range get_successors() {
            return graph.get_successors(graph.nodes[node_id]);
        }

        virtual ~ProxyNode() = default;

        Graph<DataT>& graph;
        NodeId node_id;
    };

//--------------------------------------------------------------------------------

    template <typename DataT>
    class SubGraph {
        SubGraph(Graph<DataT>& graph_, llvm::SmallVector<NodeId, 128>& nodes_):
            graph(graph);
            nodes(nodes_);    
        {}

        Graph<DataT>& graph;
        llvm::SmallVector<NodeId, 128>& nodes;
    };
}