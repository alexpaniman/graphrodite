#pragma once
#include <stdexcept>
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
    concept VectorConcept = vector_traits<std::decay_t<T>>::value; 
    //                                    ^~~~~~~~~~~~~~~
    // You need to use std::decay or std::remove_cv, so it works
    // even for reference or qualified types, for example for:
    // std::vector<int>& or const std::vector<int>& ...

    // Without this addition this concept would fail even if you
    // dealt with that recursive instantiation issue.

    template <typename T>
    concept GraphStorageConcept = requires(T graph) {
        { graph.nodes } -> VectorConcept;
    };


    template <typename DataT, GraphStorageConcept StorageT>
    class traverse_range;

    template <typename DataT, GraphStorageConcept StorageT>
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

    template <typename DataT, GraphStorageConcept StorageT>
    class Node {
    
    public:
        Node(StorageT& graph_, NodeId node_id_):
            graph(graph_),
            node_id(node_id_) {}

        successors_range<DataT, StorageT> get_successors() {
            // Now it's this node's job to construct
            // successors range, having information
            // only about BasicGraph:

            // So can't do this anymore:
            // return graph.get_successors(*this);

            // I'm feel to sleepy to implement now,
            // but I believe in you!
            throw std::runtime_error("Unimplemented!");
        }

        DataT get_data() {
            return graph.nodes[node_id].data;
        }

        NodeId id() { return node_id; }

    private:
        StorageT& graph;
        NodeId node_id;
    };

//--------------------------------------------------------------------------------

    // The idea is that our previous Graph is actually a convenient wrapper that 
    // operates in terms of ProxyNodes (or as we call the now -- just Nodes).

    // This is indeed convenient, but it's hard to create concept for, because it
    // will cause recursive instantiation of Graph.

    // So instead of that, let's separate the core part of Graph, that still
    // satisfies Graph concept and is sufficient for all iterators.

    // We will inherit from it and add all whistles and bells around to make it
    // convenient to works with (like nodes() for iterating over all proxy nodes)

    // This pattern actually has a name: shell/kernel if i remember correctly,
    // in our case BasicGraph is kernel and shell is, well, "regular" Graph.
    template <typename DataT>
    class BasicGraph {
    public:
        // This, really should be an allocator, something like free_list<dyn_array<mallocator>>,
        // but I'm yet to make such, but for now this should be enough to illustrate the idea:  
        std::vector<details::RawNode<DataT>> nodes;

        // It's internal API can look something like:
        NodeId successors(NodeId target); // Needs implementing! It should use ProxyNodes.
    };

    template <typename DataT>
    class Graph: public BasicGraph<DataT> {
    
    public:
        BasicGraph<DataT> storage;

        NodeId create_node(DataT data) { 
            storage.nodes.emplace_back(data); 
            return storage.nodes.size() - 1;
        }
        
        traverse_range<DataT, BasicGraph<DataT>> traverse() { return { this->storage }; }

        successors_range<DataT, BasicGraph<DataT>> get_successors(Node<DataT, BasicGraph<DataT>>& node) {
            assert(node.id() >= 0 and node.id() < storage.nodes.size() and "Incorrect node");
            return { *this, node.id() };
        }

        void dump() {
            for (int i = 0; i < storage.nodes.size(); ++ i) {
                std::cout << "node: " << i << ": ";
                for (auto &&succ: storage.nodes[i].successors)
                    std::cout << succ << ", ";
                std::cout << "\n";
            }
        }

    };
} // namespace graph
