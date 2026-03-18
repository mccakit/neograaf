module;
export module graaf:cycle_detection;

import std;
import :types;
import :graph;

export namespace graaf::algorithm {

// ---------------------------------------------------------------------------
// detail
// ---------------------------------------------------------------------------

namespace detail {

enum class vertex_color { UNVISITED, VISITED, NO_CYCLE };

template <typename V, typename E>
bool do_dfs_directed(
    const graph<V, E, graph_type::DIRECTED>& graph,
    std::unordered_map<vertex_id_t, vertex_color>& colored_vertices,
    vertex_id_t current) {
  colored_vertices[current] = vertex_color::VISITED;

  for (const auto& neighbour : graph.get_neighbors(current)) {
    if (colored_vertices[neighbour] == vertex_color::UNVISITED) {
      if (do_dfs_directed(graph, colored_vertices, neighbour)) return true;
    } else if (colored_vertices[neighbour] == vertex_color::VISITED) {
      return true;
    }
  }

  colored_vertices[current] = vertex_color::NO_CYCLE;
  return false;
}

template <typename V, typename E>
bool do_dfs_undirected(
    const graph<V, E, graph_type::UNDIRECTED>& graph,
    std::unordered_map<vertex_id_t, bool>& visited_vertices,
    std::unordered_map<vertex_id_t, vertex_id_t>& parent_vertices,
    vertex_id_t parent_vertex, vertex_id_t current) {
  visited_vertices[current] = true;

  for (const auto& neighbour : graph.get_neighbors(current)) {
    if (neighbour == parent_vertex) continue;

    if (visited_vertices[neighbour]) return true;

    parent_vertices[neighbour] = parent_vertex;

    if (do_dfs_undirected(graph, visited_vertices, parent_vertices,
                          neighbour, parent_vertices[neighbour])) {
      return true;
    }
  }

  return false;
}

}  // namespace detail

// ---------------------------------------------------------------------------
// dfs_cycle_detection — directed
// ---------------------------------------------------------------------------

template <typename V, typename E>
[[nodiscard]] bool dfs_cycle_detection(
    const graph<V, E, graph_type::DIRECTED>& graph) {
  std::unordered_map<vertex_id_t, detail::vertex_color> colored_vertices{};

  for (const auto& [vertex_id, _] : graph.get_vertices()) {
    using enum detail::vertex_color;
    if (colored_vertices[vertex_id] == UNVISITED &&
        detail::do_dfs_directed(graph, colored_vertices, vertex_id)) {
      return true;
    }
  }

  return false;
}

// ---------------------------------------------------------------------------
// dfs_cycle_detection — undirected
// ---------------------------------------------------------------------------

template <typename V, typename E>
[[nodiscard]] bool dfs_cycle_detection(
    const graph<V, E, graph_type::UNDIRECTED>& graph) {
  if (graph.edge_count() >= graph.vertex_count() && graph.vertex_count() > 0) {
    return true;
  }

  std::unordered_map<vertex_id_t, bool> visited_vertices{};
  std::unordered_map<vertex_id_t, vertex_id_t> parent_vertices{};

  for (const auto& [vertex_id, _] : graph.get_vertices()) {
    if (!visited_vertices.contains(vertex_id) &&
        detail::do_dfs_undirected(graph, visited_vertices, parent_vertices,
                                  vertex_id, parent_vertices[vertex_id])) {
      return true;
    }
  }

  return false;
}

}  // namespace graaf::algorithm
