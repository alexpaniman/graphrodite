#pragma once

namespace graph {
    template <typename DataT>
    class successors_range {
    
    public:
        successors_range(Graph<DataT>& graph, NodeId node_id):
            begin(graph, node_id),
            end(graph, graph.nodes[node_id].successors.size() + 1) // TODO do I need +1 here?
        { }
        
        successors_iter<DataT> begin, end;

        virtual ~successors_range() = default;
    };

//--------------------------------------------------------------------------------
    
    template <typename DataT>
    class successors_iter {

    public:
        successors_iter(NodeId current_node_id_, Graph<DataT>& graph_):
            current_node_id(current_node_id_),
            graph(graph_) 
        { }   
        
        successors_iter<DataT>& operator+=(int append_value) {
            Node current_node = graph.nodes[current_node_id];
            this->target_successor_id += append_value;
            
            NodeId next_succsessor_id = current_node.successors[target_successor_id];         
            this->current_node_id = next_succsessor_id;

            return *this;
        }

        bool operator==(successors_iter<DataT>& other) { // TODO две ноды из разных графов могут иметь один ID
            return (this->current_node_id == other->current_node_id);
        }
        
        bool operator!=(successors_iter<DataT>& other) {
            return !this->operator==(other);
        }

        successors_iter<DataT>& operator++() { // TODO что если дошли до конца списка successors
            return *this += 1;
        }

        successors_iter<DataT> operator++(int) { 
            successors_iter<DataT> tmp = *this;
            this->operator++();

            return tmp;
        }

        Node<DataT>& operator*() {
            return graph.nodes[current_node_id];
        }

        virtual ~successors_iter() = default;

        NodeId current_node_id;
        NodeId target_successor_id = 0;
        Graph<DataT>& graph;
    };

//--------------------------------------------------------------------------------

    template <typename DataT>
    class traverse_range {
    
    public:
        traverse_range(Graph<DataT>& graph):
            begin(graph, 0),
            end(graph, graph.nodes.size() + 1) // TODO do I need +1 here? 
        { }

        traverse_iter<DataT> begin, end;

        virtual ~traverse_range() = default;
    };

    template <typename DataT>
    class traverse_iter {
    
    public:
        traverse_iter(Graph<DataT>& graph_):
            graph(graph_),
            current_node_id(0)
        { }

        bool operator==(traverse_iter& other) {
            return this->current_node_id == other->current_node_id;  
        }

        bool operator!=(traverse_iter& other) {
            return !operator==(other);  
        }

        traverse_iter<DataT>& operator+=(int append_value) { // TODO что если выйдем за границы... или похуй....
            this->current_node_id += append_value;

            return *this;
        }   

        traverse_iter<DataT>& operator++() {
            return this+=1;
        }

        traverse_iter<DataT> operator++(int) {
            traverse_iter<DataT> tmp = *this;
            *this+=1;
            return tmp;
        }

        Node<DataT>& operator*() { 
            return graph.nodes[current_node_id];
        }

        Graph<DataT>& graph;
        NodeID current_node_id;

        virtual ~traverse_iter() = default;
    };
} // namespace graph