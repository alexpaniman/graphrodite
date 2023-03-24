#pragma once
#include "graph.h"

namespace graph_lib {
    template <GraphStorageConcept StorageT>
    class successors_iter {

    public:
        successors_iter(StorageT& storage_, NodeId current_node_id_, int target_successor_id_): 
            storage(storage_),
            current_node_id(current_node_id_),
            target_successor_id(target_successor_id_) {}   
        
        successors_iter<StorageT>& operator+=(int append_value) {
            target_successor_id += append_value;
            return *this;
        }

        successors_iter<StorageT>& operator++() { return (*this) += 1; }

        successors_iter<StorageT> operator++(int) { 
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



        Node<StorageT> operator*() {
            return { storage, storage.nodes[current_node_id].successors[target_successor_id] };
        }

    private:
        StorageT& storage;
        NodeId current_node_id;

        int target_successor_id = 0;
    };

//--------------------------------------------------------------------------------
 
    template <GraphStorageConcept StorageT>
    class successors_range {
    
    public:
        successors_range(StorageT& storage_, NodeId node_id) : 
            storage(storage_), 
            start_node_id(node_id) {}
        
        successors_iter<StorageT> begin() { return { storage, start_node_id,                                                                   0 }; }
        successors_iter<StorageT>   end() { return { storage, start_node_id, static_cast<NodeId>(storage.nodes[start_node_id].successors.size()) }; }

    private:
        StorageT& storage;
        NodeId start_node_id;
    };

//--------------------------------------------------------------------------------

    template <GraphStorageConcept StorageT>
    class traverse_iter {
    
    public:
        traverse_iter(StorageT& storage_, NodeId current_node_id_):
            storage(storage_),
            current_node_id(current_node_id_) {}
        
        traverse_iter<StorageT>& operator+=(int append_value) {
            current_node_id += append_value;
            return *this;
        }

        traverse_iter<StorageT>& operator++() { return (*this) += 1; }

        traverse_iter<StorageT> operator++(int) { 
            auto tmp = *this;
            (*this)++;

            return tmp;
        }

        std::strong_ordering operator<=>(const traverse_iter<StorageT> &other) const {
            return current_node_id <=> other.current_node_id;
        }

#define OP(name) \
        bool operator name(const traverse_iter<StorageT> &other) const { return ((*this) <=> other) name 0; };
        OP(<=) OP(>=) OP(==) OP(!=) OP(<) OP(>)
#undef OP

        Node<StorageT> operator*() { return { storage, current_node_id }; }

    private:
        StorageT& storage;
        NodeId current_node_id;
    };


//--------------------------------------------------------------------------------
    template <GraphStorageConcept StorageT>
    class traverse_range {
    
    public:
        traverse_range(StorageT& storage_): storage(storage_) {}

        traverse_iter<StorageT> begin() { return { storage,                                         0 }; }
        traverse_iter<StorageT>   end() { return { storage, static_cast<NodeId>(storage.nodes.size()) }; }

    private:
        StorageT& storage;
    };

} // namespace graph
