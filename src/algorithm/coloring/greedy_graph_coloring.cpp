module;

export module graaf:graph_coloring;

import std;
import :graph;

export namespace graaf::algorithm {

template <typename GRAPH>
[[nodiscard]] std::unordered_map<vertex_id_t, int> greedy_graph_coloring(
    const GRAPH& graph) {
  std::unordered_map<vertex_id_t, int> coloring{};

  for (const auto& [vertex_id, _] : graph.get_vertices()) {
    int available_color{0};

    for (const auto neighbor_id : graph.get_neighbors(vertex_id)) {
      if (coloring.contains(neighbor_id)) {
        const auto neighbor_color{coloring.at(neighbor_id)};
        if (neighbor_color >= available_color) {
          available_color = neighbor_color + 1;
        }
      }
    }

    coloring[vertex_id] = available_color;
  }

  return coloring;
}

}  // namespace graaf::algorithm
