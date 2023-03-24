#include "graph.h"
#include "graph_iters.h"
#include "graph_builder.h"
#include <vector>

int main(void) {
    std::vector<std::vector<int>> graph_data = {
        {1, 2},
        {3},
        {3},
        {}
    };

    graph_lib::GraphBuilder<int> builder;

    for (int idx = 0; idx < graph_data.size(); ++idx) {
        builder.add_vertex(idx);

        for (auto& next : graph_data[idx]) {
            builder.add_vertex(next);            
            builder.add_edge(idx, next);
        }
    }

    auto graph = builder.get_graph();
    graph.dump();

    // Now concepts work! Yay! Both of them are Graphs:
    static_assert(graph_lib::GraphStorageConcept<graph_lib::BasicGraph<int>>);
    static_assert(graph_lib::GraphStorageConcept<graph_lib::Graph<int>>);

    for (auto next_node : graph.traverse()) {
        std::cout << next_node.id() << "\n";
    }

    return 0;
}
