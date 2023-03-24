#pragma once
#include "graph.h"
#include "graph_iters.h"
#include "assert.h"

namespace graph_lib {
    template <typename DataT>
    class SubGraph {
        
        using storage_t = GraphStorage<DataT>;
        
    public:

        SubGraph(Graph<DataT>& graph_):
            storage(graph_.storage) {}

        void add_node(Node<storage_t> node) {
            assert(node.id() >= 0 and node.id() < storage.nodes.size() and "Incorrect node");
            
            nodes.emplace_back(node.id());
        }

        traverse_range<storage_t> traverse() {
            return {storage};
        }
        
        void dump() {
            for (auto node_id : nodes) {
                std::cout << "node: " << node_id << ": ";
                for (auto &&succ: storage.nodes[node_id].successors)
                    std::cout << succ << ", ";
                std::cout << "\n";
            }
        }

    private:
        storage_t &storage;
        std::vector<NodeId> nodes;
    };
}