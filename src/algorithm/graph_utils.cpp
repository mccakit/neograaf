module;

export module graaf:algorithm_utils;

import std;
import :graph;

export namespace graaf {

template <typename VERTEX_T, typename EDGE_T>
[[nodiscard]] directed_graph<VERTEX_T, EDGE_T> get_transposed_graph(
    const directed_graph<VERTEX_T, EDGE_T>& graph) {
  directed_graph<VERTEX_T, EDGE_T> transposed{};

  for (const auto& [edge_id, edge] : graph.get_edges()) {
    const auto [u, v]{edge_id};

    if (!transposed.has_vertex(u)) {
      transposed.add_vertex(graph.get_vertex(u), u);
    }
    if (!transposed.has_vertex(v)) {
      transposed.add_vertex(graph.get_vertex(v), v);
    }

    transposed.add_edge(v, u, edge);
  }

  // Isolated vertices (no edges) are not captured above — add them too
  for (const auto& [vertex_id, vertex] : graph.get_vertices()) {
    if (!transposed.has_vertex(vertex_id)) {
      transposed.add_vertex(vertex, vertex_id);
    }
  }

  return transposed;
}

}  // namespace graaf
