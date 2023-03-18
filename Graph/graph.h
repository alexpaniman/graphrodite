#pragma once
#include <llvm/ADT/SmallVector.h>
#include <vector>
#include <inttypes.h>
#include <iostream>
#include <llvm/Support/raw_ostream.h>

//--------------------------------------------------------------------------------

namespace graph_lib {
    template <typename DataT>
    class traverse_range;

    template <typename DataT>
    class successors_range;
    
    template <typename DataT>
    class Graph;
    
    using NodeId = int;

    namespace details {
        template <typename DataT>
        struct RawNode {
            RawNode(DataT data_): data(data_) {}

            DataT data;
            llvm::SmallVector<NodeId, 8> successors;
        };
    };

//--------------------------------------------------------------------------------

    template <typename DataT>
    class Node {
    
    public:
        Node(Graph<DataT>& graph_, NodeId node_id_):
            graph(graph_),
            node_id(node_id_)    
        { }

        successors_range<DataT> get_successors() {
            return graph.get_successors(*this);
        }

        DataT get_data() {
            return graph.nodes[node_id].data;
        }

        NodeId id() { return node_id; }

    private:
        Graph<DataT>& graph;
        NodeId node_id;
    };
//--------------------------------------------------------------------------------

    template <typename DataT>
    class Graph {
    
    public:
        NodeId create_node(DataT data) { 
            nodes.emplace_back(data); 
            return nodes.size() - 1;
        }
        
        traverse_range<DataT> traverse() { return { *this }; }

        successors_range<DataT> get_successors(Node<DataT>& node) {
            // NodeId node_id = reinterpret_cast<intptr_t>(&node) - reinterpret_cast<intptr_t>(&nodes[0]);
            llvm::errs() << "NODE ID: " << node.id() << "\n";


            assert(node.id() >= 0 and node.id() < nodes.size() and "Tobi pezda");
            // if (node.id() < 0 or node.id() >= nodes.size())
            //     return successors_range<DataT>(*this, node.id(), false);

            return { *this, node.id() };
        }

        void dump() {

            for (int i = 0; i < nodes.size(); ++ i) {
                llvm::errs() << "node: " << i << ": ";
                for (auto &&succ: nodes[i].successors)
                    llvm::errs() << succ << ", ";
                llvm::errs() << "\n";
            }
            
        }

        std::vector<details::RawNode<DataT>> nodes;
    };


} // namespace graph