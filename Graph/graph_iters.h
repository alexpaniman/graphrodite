#pragma once
#include "graph.h"

namespace graph_lib {
    template <typename DataT, GraphStorageConcept StorageT>
    class successors_iter {

    public:
        successors_iter(Graph<DataT>& graph_, NodeId current_node_id_, int target_successor_id_): 
            graph(graph_),
            current_node_id(current_node_id_),
            target_successor_id(target_successor_id_) {}   
        
        successors_iter<DataT, StorageT>& operator+=(int append_value) {
            target_successor_id += append_value;
            return *this;
        }

        successors_iter<DataT, StorageT>& operator++() { return (*this) += 1; }

        successors_iter<DataT, StorageT> operator++(int) { 
            auto tmp = *this;
            (*this)++;

            return tmp;
        }


        auto operator<=>(const successors_iter &other) const {
            return target_successor_id <=> other.target_successor_id;
        }

#define OP(name) \
        bool operator name(const successors_iter &other) const { return ((*this) <=> other) name 0; };
        OP(<=) OP(>=) OP(==) OP(!=) OP(<) OP(>)
#undef OP


        Node<DataT, StorageT> operator*() {
            return { graph, graph.nodes[current_node_id].successors[target_successor_id] };
        }

    private:
        StorageT& graph;
        NodeId current_node_id;

        int target_successor_id = 0;
    };

//--------------------------------------------------------------------------------
 
    template <typename DataT, GraphStorageConcept StorageT>
    class successors_range {
    
    public:
        successors_range(Graph<DataT>& graph_, NodeId node_id) : 
            graph(graph_), 
            start_node_id(node_id) {}
        
        successors_iter<DataT, StorageT> begin() { return { graph, start_node_id,                                                                 0 }; }
        successors_iter<DataT, StorageT>   end() { return { graph, start_node_id, static_cast<NodeId>(graph.nodes[start_node_id].successors.size()) }; }

    private:
        StorageT& graph;
        NodeId start_node_id;
    };

//--------------------------------------------------------------------------------

    template <typename DataT, GraphStorageConcept StorageT>
    class traverse_iter {
    
    public:
        traverse_iter(StorageT& graph_, NodeId current_node_id_):
            graph(graph_),
            current_node_id(current_node_id_) {}
        

        traverse_iter<DataT, StorageT>& operator+=(int append_value) {
            current_node_id += append_value;
            return *this;
        }

        traverse_iter<DataT, StorageT>& operator++() { return (*this) += 1; }

        traverse_iter<DataT, StorageT> operator++(int) { 
            auto tmp = *this;
            (*this)++;

            return tmp;
        }


        std::strong_ordering operator<=>(const traverse_iter<DataT, StorageT> &other) const {
            return current_node_id <=> other.current_node_id;
        }

#define OP(name) \
        bool operator name(const traverse_iter<DataT, StorageT> &other) const { return ((*this) <=> other) name 0; };
        OP(<=) OP(>=) OP(==) OP(!=) OP(<) OP(>)
#undef OP

        Node<DataT, StorageT> operator*() { return { graph, current_node_id }; }

    private:
        StorageT& graph;
        NodeId current_node_id;
    };


//--------------------------------------------------------------------------------
    template <typename DataT, GraphStorageConcept StorageT>
    class traverse_range {
    
    public:
        traverse_range(StorageT& graph_): graph(graph_) {}

        traverse_iter<DataT, StorageT> begin() { return { graph,                                       0 }; }
        traverse_iter<DataT, StorageT>   end() { return { graph, static_cast<NodeId>(graph.nodes.size()) }; }

    private:
        StorageT& graph;
    };

} // namespace graph
