#pragma once
#include <vector>
#include <inttypes.h>
#include <iostream>

//--------------------------------------------------------------------------------

namespace graph_lib {
    template <typename T> struct vector_traits {
        static bool const value = false;  
    };

    template <typename... Args>
    struct vector_traits<std::vector<Args...>> {
        static bool const value = true;
    };

    template <typename T>
    concept VectorConcept = vector_traits<T>::value; 

    template <typename T>
    concept GraphConcept = requires(T graph) {
        {graph.nodes} -> VectorConcept;
    };


    template <typename DataT, GraphConcept GraphT>
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
            std::vector<NodeId> successors;
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
        
        traverse_range<DataT, Graph> traverse() { return { *this }; }

        successors_range<DataT> get_successors(Node<DataT>& node) {
            assert(node.id() >= 0 and node.id() < nodes.size() and "Incorrect node");
            return { *this, node.id() };
        }

        void dump() {
            for (int i = 0; i < nodes.size(); ++ i) {
                std::cout << "node: " << i << ": ";
                for (auto &&succ: nodes[i].successors)
                    std::cout << succ << ", ";
                std::cout << "\n";
            }
        }

        std::vector<details::RawNode<DataT>> nodes;
    };
} // namespace graph