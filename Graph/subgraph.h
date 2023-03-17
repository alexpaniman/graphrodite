
#pragma once
#include "graph.h"

namespace graph {
    template <typename DataT>
    class SubGraph {
        SubGraph(Graph<DataT>& graph_, std::vector<NodeId>& nodes_):
            graph(graph);
            nodes(nodes_);    
        {}

        Graph<DataT>& graph;
        std::vector<NodeId> nodes;
    };
}