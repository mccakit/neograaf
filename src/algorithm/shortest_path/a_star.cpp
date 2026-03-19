module;

export module graaf:a_star;

import std;
import :graph;
import :edge;
import :shortest_path_common;

export namespace graaf::algorithm {

template <typename V, typename E, graph_type T, typename HEURISTIC_T,
          typename WEIGHT_T = decltype(get_weight(std::declval<E>()))>
  requires std::is_invocable_r_v<WEIGHT_T, HEURISTIC_T&, vertex_id_t>
[[nodiscard]] std::optional<graph_path<WEIGHT_T>> a_star_search(
    const graph<V, E, T>& graph,
    vertex_id_t start_vertex,
    vertex_id_t target_vertex,
    const HEURISTIC_T& heuristic) {
  using weighted_path_item = detail::path_vertex<WEIGHT_T>;
  using a_star_queue_t =
      std::priority_queue<weighted_path_item,
                          std::vector<weighted_path_item>,
                          std::greater<>>;

  a_star_queue_t open_set{};
  std::unordered_map<vertex_id_t, WEIGHT_T> g_score;
  std::unordered_map<vertex_id_t, weighted_path_item> vertex_info;

  g_score[start_vertex]    = 0;
  vertex_info[start_vertex] = {start_vertex, heuristic(start_vertex),
                                start_vertex};
  open_set.push(vertex_info[start_vertex]);

  while (!open_set.empty()) {
    auto current{open_set.top()};
    open_set.pop();

    if (current.id == target_vertex) {
      return reconstruct_path(start_vertex, target_vertex, vertex_info);
    }

    for (const auto& neighbor : graph.get_neighbors(current.id)) {
      const WEIGHT_T edge_weight{
          get_weight(graph.get_edge(current.id, neighbor))};

      if (edge_weight < 0) {
        throw std::invalid_argument{
            std::format("Negative edge weight [{}] between vertices [{}] -> [{}].",
                        edge_weight, current.id, neighbor)};
      }

      const WEIGHT_T tentative_g{g_score[current.id] + edge_weight};

      if (!vertex_info.contains(neighbor) || tentative_g < g_score[neighbor]) {
        g_score[neighbor]    = tentative_g;
        const auto f_score   = tentative_g + heuristic(neighbor);
        vertex_info[neighbor] = {neighbor, f_score, current.id};
        open_set.push(vertex_info[neighbor]);
      }
    }
  }

  return std::nullopt;
}

}  // namespace graaf::algorithm
