module;

export module graaf:welsh_powell;

import std;
import :graph;
import :vertex_properties;

export namespace graaf::algorithm {

template <typename GRAPH>
[[nodiscard]] std::unordered_map<vertex_id_t, int> welsh_powell_coloring(
    const GRAPH& graph) {
  using degree_vertex_pair = std::pair<int, vertex_id_t>;

  std::vector<degree_vertex_pair> degree_vertex_pairs;
  for (const auto& [vertex_id, _] : graph.get_vertices()) {
    degree_vertex_pairs.emplace_back(
        properties::vertex_degree(graph, vertex_id), vertex_id);
  }

  std::ranges::sort(degree_vertex_pairs, std::ranges::greater{});

  std::unordered_map<vertex_id_t, int> color_map;

  for (const auto& [_, vertex_id] : degree_vertex_pairs) {
    int color{0};

    for (const auto& neighbor : graph.get_neighbors(vertex_id)) {
      if (color_map.contains(neighbor) && color_map[neighbor] == color) {
        ++color;
      }
    }

    color_map[vertex_id] = color;
  }

  return color_map;
}

}  // namespace graaf::algorithm
