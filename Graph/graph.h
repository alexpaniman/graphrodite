#pragma once

#include <vector>
#include <inttypes.h>
#include <iostream>

//--------------------------------------------------------------------------------

namespace graph_lib {

    using NodeId = int;

}

namespace graph_lib::details {

    template <typename DataT>
    struct RawNode {
        RawNode(DataT data_): data(data_) {}

        DataT data;
        std::vector<NodeId> successors;
    };

};

namespace graph_lib {

    template <typename DataT>
    class Graph;


    template <typename DataT>
    class successors_range;

    template <typename DataT>
    class Node {
    public:
        Node(Graph<DataT>& graph_, NodeId node_id_):
            graph(graph_),
            node_id(node_id_) {}

        successors_range<DataT> get_successors();
        DataT get_data();

        NodeId id() { return node_id; }

    private:
        Graph<DataT>& graph;
        NodeId node_id;
    };


    template <typename DataT>
    class successors_iter {
    public:
        successors_iter(Graph<DataT>& graph_, NodeId current_node_id_, int target_successor_id_): 
            graph(graph_),
            current_node_id(current_node_id_),
            target_successor_id(target_successor_id_) {}   
        
        // ==============> increment

        successors_iter<DataT>& operator+=(int append_value) {
            target_successor_id += append_value;
            return *this;
        }

        successors_iter<DataT>& operator++() { return (*this) += 1; }
        successors_iter<DataT> operator++(int) { 
            auto tmp = *this;
            (*this)++;

            return tmp;
        }

        // ==============> compare

        auto operator<=>(const successors_iter<DataT> &other) const {
            return target_successor_id <=> other.target_successor_id;
        }

        traverse_iter<DataT> begin() { return { graph,                                       0 }; }
        traverse_iter<DataT>   end() { return { graph, static_cast<NodeId>(graph.nodes.size()) }; }

        // ==============> dereference

        Node<DataT> successors_iter<DataT>::operator*();

        int target_successor_id = 0;
    };

//--------------------------------------------------------------------------------
 
    template <typename DataT>
    class successors_range {
    public:
        successors_range(Graph<DataT>& graph_, NodeId node_id):
            graph(graph_),
            start_node_id(node_id) {}
        
        successors_iter<DataT> begin();
        successors_iter<DataT>   end();

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


        auto operator<=>(const traverse_iter<DataT> &other) const {
            return current_node_id <=> other.current_node_id;
        }

        #define OP(name) \
            bool operator name(const traverse_iter<DataT> &other) const { return ((*this) <=> other) name 0; }

        OP(<=) OP(>=) OP(==) OP(!=) OP(<) OP(>)
        #undef OP


        Node<DataT> operator*();

    private:
        Graph<DataT>& graph;
        NodeId current_node_id;
    };


    //--------------------------------------------------------------------------------

    template <typename T> struct vector_traits {
        inline static constexpr bool value = false;  
    };

    template <typename... Args>
    struct vector_traits<std::vector<Args...>> {
        inline static constexpr bool value = true;
    };

    template <typename T>
    concept VectorConcept = vector_traits<T>::value; 

    template <typename T>
    concept GraphConcept = requires(T graph) {
        { graph.get_nodes() };
    };

    template <typename DataT, GraphConcept GraphT>
    class traverse_range {
    public:
        traverse_range(GraphT& graph_): graph(graph_) {}

        traverse_iter<DataT> begin() { return { graph,                                       0 }; }
        traverse_iter<DataT>   end() { return { graph, static_cast<NodeId>(graph.nodes.size()) }; }

    private:
        GraphT& graph;
    };

    template <typename DataT>
    class Graph {
    public:
        std::vector<details::RawNode<DataT>>& get_nodes();

        NodeId create_node(DataT data);

        traverse_range<DataT, Graph<DataT>> traverse();
        successors_range<DataT> get_successors(Node<DataT>& node);

        void dump();
        std::vector<details::RawNode<DataT>> nodes;
    };

//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------

    template <typename DataT>
    std::vector<details::RawNode<DataT>>& Graph<DataT>::get_nodes() {
        return nodes;
    }

    template <typename DataT>
    NodeId Graph<DataT>::create_node(DataT data) { 
        nodes.emplace_back(data); 
        return nodes.size() - 1;
    }
    
    template <typename DataT>
    traverse_range<DataT, Graph<DataT>> Graph<DataT>::traverse() { return { *this }; }

    template <typename DataT>
    successors_range<DataT> Graph<DataT>::get_successors(Node<DataT>& node) {
        assert(node.id() >= 0 and node.id() < nodes.size() and "Incorrect node");
        return { *this, node.id() };
    }

    template <typename DataT>
    void Graph<DataT>::dump() {
        for (int i = 0; i < nodes.size(); ++ i) {
            std::cout << "node: " << i << ": ";
            for (auto &&succ: nodes[i].successors)
                std::cout << succ << ", ";
            std::cout << "\n";
        }
    }


    template <typename DataT>
    Node<DataT> successors_iter<DataT>::operator*() {
        return { graph, graph.nodes[current_node_id].successors[target_successor_id] };
    }

    template <typename DataT>
    Node<DataT> traverse_iter<DataT>::operator*() { return { graph, current_node_id }; }

    template <typename DataT>
    traverse_iter<DataT> traverse_iter<DataT>::begin() { return { graph,                                       0 }; }
    
    template <typename DataT>
    traverse_iter<DataT> traverse_iter<DataT>::  end() { return { graph, static_cast<NodeId>(graph.nodes.size()) }; }


    template <typename DataT>
    successors_range<DataT> Node<DataT>::get_successors() {
        return graph.get_successors(*this);
    }

    template <typename DataT>
    DataT Node<DataT>::get_data() { return graph.nodes[node_id].data; }

    template <typename DataT>
    successors_iter<DataT> successors_range<DataT>::begin() {
        return { graph, start_node_id,                                                                 0 };
    }

    template <typename DataT>
    successors_iter<DataT> successors_range<DataT>::  end() {
        return { graph, start_node_id, static_cast<NodeId>(graph.nodes[start_node_id].successors.size()) };
    }

} // namespace graph