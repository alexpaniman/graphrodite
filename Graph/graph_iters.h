#pragma once
#include "graph.h"

namespace graph_lib {
    template <typename DataT>
    class successors_iter {

    public:
        successors_iter(Graph<DataT>& graph_, NodeId current_node_id_, int target_successor_id_): 
            graph(graph_),
            current_node_id(current_node_id_),
            target_successor_id(target_successor_id_) {}   
        
        successors_iter<DataT>& operator+=(int append_value) {
            target_successor_id += append_value;
            return *this;
        }

        successors_iter<DataT>& operator++() { return (*this) += 1; }

        successors_iter<DataT> operator++(int) { 
            successors_iter<DataT> tmp = *this;
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


        Node<DataT> operator*() {
            return { graph, graph.nodes[current_node_id].successors[target_successor_id] };
        }

    private:
        Graph<DataT>& graph;
        NodeId current_node_id;

        int target_successor_id = 0;
    };

//--------------------------------------------------------------------------------
 
    template <typename DataT>
    class successors_range {
    
    public:
        successors_range(Graph<DataT>& graph_, NodeId node_id) : 
            graph(graph_), 
            start_node_id(node_id) {}
        
        successors_iter<DataT> begin() { return { graph, start_node_id,                                                                 0 }; }
        successors_iter<DataT>   end() { return { graph, start_node_id, static_cast<NodeId>(graph.nodes[start_node_id].successors.size()) }; }

    private:
        Graph<DataT>& graph;
        NodeId start_node_id;
    };

//--------------------------------------------------------------------------------

    template <typename DataT>
    class traverse_iter {
    
    public:
        traverse_iter(Graph<DataT>& graph_, NodeId current_node_id_):
            graph(graph_),
            current_node_id(current_node_id_) {}
        

        traverse_iter<DataT>& operator+=(int append_value) {
            current_node_id += append_value;
            return *this;
        }

        traverse_iter<DataT>& operator++() { return (*this) += 1; }

        traverse_iter<DataT> operator++(int) { 
            traverse_iter<DataT> tmp = *this;
            (*this)++;

            return tmp;
        }


        std::strong_ordering operator<=>(const traverse_iter<DataT> &other) const {
            return current_node_id <=> other.current_node_id;
        }

#define OP(name) \
        bool operator name(const traverse_iter<DataT> &other) const { return ((*this) <=> other) name 0; };
        OP(<=) OP(>=) OP(==) OP(!=) OP(<) OP(>)
#undef OP

        Node<DataT> operator*() { return { graph, current_node_id }; }

    private:
        Graph<DataT>& graph;
        NodeId current_node_id;
    };


//--------------------------------------------------------------------------------
    template <typename DataT>
    class traverse_range {
    
    public:
        traverse_range(Graph<DataT>& graph_): graph(graph_) {}

        traverse_iter<DataT> begin() { return { graph,                                       0 }; }
        traverse_iter<DataT>   end() { return { graph, static_cast<NodeId>(graph.nodes.size()) }; }

    private:
        Graph<DataT>& graph;
    };

} // namespace graph