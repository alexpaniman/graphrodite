#pragma once
#include <llvm/ADT/SmallVector.h>
#include <vector>
#include "graph_iters.h"

//--------------------------------------------------------------------------------

namespace graph {
    using NodeId = int;

    template <typename DataT>
    struct Node {
        Node(DataT data_): data(data_) {}

        DataT data;
        llvm::SmallVector<NodeId, 128> successors;
        virtual ~Node() = default;
    };

    template <typename DataT>
    class Graph {
    
    public:
        NodeId create_node(DataT data) { 
            nodes.append(Node(data)); 
            return nodes.size();
        }
        
        traverse_range<DataT> traverse() { 
            return traverse_range<DataT>(*this);
        }

        proxy_node_range<DataT> nodes() { 
            return proxy_node_range<DataT>(); // TODO what is proxy_node_range? How its different from traverse_range/successors_range
        }
        
        successors_range<DataT> get_successors(Node<DataT> node) {
            auto search_result = std::find(nodes.begin(), nodes.end(), node);
            
            if (search_result != nodes.end())
                return successors_range<DataT>(*this, *search_result - nodes.begin());
            
            return successors_range<DataT>(*this, nodes.end()); // TODO ok?
        }

        virtual ~Graph() = default;

        llvm::SmallVector<Node<DataT>, 512> nodes;
    };

} // namespace graph