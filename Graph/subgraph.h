
#pragma once
#include "graph.h"

namespace graph_lib {
    template <typename DataT>
    class SubGraph {
        SubGraph(Graph<DataT>& graph_, std::vector<NodeId>& nodes_):
            graph(graph_),
            nodes(nodes_)    
        {}

        Graph<DataT>& graph;
        std::vector<NodeId> nodes;
    };
}