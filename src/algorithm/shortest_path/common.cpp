module;

export module graaf:shortest_path_common;

import std;
import :types;

export namespace graaf::algorithm {

template <typename WEIGHT_T>
struct graph_path {
  std::list<vertex_id_t> vertices;
  WEIGHT_T total_weight;

  bool operator==(const graph_path& other) const {
    return vertices == other.vertices && total_weight == other.total_weight;
  }
};

namespace detail {

template <typename WEIGHT_T>
struct path_vertex {
  vertex_id_t id;
  WEIGHT_T    dist_from_start;
  vertex_id_t prev_id;

  [[nodiscard]] bool operator>(const path_vertex& other) const {
    return dist_from_start > other.dist_from_start;
  }
};

template <typename WEIGHT_T>
[[nodiscard]] std::optional<graph_path<WEIGHT_T>> reconstruct_path(
    vertex_id_t start_vertex, vertex_id_t end_vertex,
    std::unordered_map<vertex_id_t, path_vertex<WEIGHT_T>>& vertex_info) {
  if (!vertex_info.contains(end_vertex)) return std::nullopt;

  graph_path<WEIGHT_T> path{};
  auto current{end_vertex};

  while (current != start_vertex) {
    path.vertices.push_front(current);
    current = vertex_info[current].prev_id;
  }

  path.vertices.push_front(start_vertex);
  path.total_weight = vertex_info[end_vertex].dist_from_start;
  return path;
}

}  // namespace detail

}  // namespace graaf::algorithm
