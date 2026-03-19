module;

export module graaf:floyd_warshall;

import std;
import :graph;
import :edge;

export namespace graaf::algorithm {

template <typename V, typename E, graph_type T,
          typename WEIGHT_T = decltype(get_weight(std::declval<E>()))>
[[nodiscard]] std::vector<std::vector<WEIGHT_T>> floyd_warshall_shortest_paths(
    const graph<V, E, T>& graph) {
  const std::size_t n{graph.vertex_count()};
  const WEIGHT_T    INF{std::numeric_limits<WEIGHT_T>::max()};
  const WEIGHT_T    ZERO{};

  std::vector<std::vector<WEIGHT_T>> dist(n, std::vector<WEIGHT_T>(n, INF));

  for (std::size_t v{0}; v < n; ++v) {
    dist[v][v] = ZERO;
  }

  for (std::size_t u{0}; u < n; ++u) {
    for (const auto& v : graph.get_neighbors(u)) {
      dist[u][v] = std::min(dist[u][v],
                            get_weight(graph.get_edge(u, v)));
    }
  }

  for (std::size_t k{0}; k < n; ++k) {
    for (std::size_t i{0}; i < n; ++i) {
      if (dist[i][k] == INF) continue;
      for (std::size_t j{0}; j < n; ++j) {
        if (dist[k][j] == INF) continue;
        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }

  return dist;
}

}  // namespace graaf::algorithm
