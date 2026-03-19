module;

export module graaf:vertex_properties;

import std;
import :graph;

export namespace graaf::properties {

template <typename V, typename E, graph_type T>
[[nodiscard]] std::size_t vertex_outdegree(const graph<V, E, T>& graph,
                                           vertex_id_t vertex_id) {
  return graph.get_neighbors(vertex_id).size();
}

template <typename V, typename E, graph_type T>
[[nodiscard]] std::size_t vertex_indegree(const graph<V, E, T>& graph,
                                          vertex_id_t vertex_id) {
  if constexpr (T == graph_type::DIRECTED) {
    return std::ranges::count_if(
        graph.get_vertices(),
        [&graph, vertex_id](const auto& kv) {
          return graph.get_neighbors(kv.first).contains(vertex_id);
        });
  } else {
    return vertex_outdegree(graph, vertex_id);
  }
}

template <typename V, typename E, graph_type T>
[[nodiscard]] std::size_t vertex_degree(const graph<V, E, T>& graph,
                                        vertex_id_t vertex_id) {
  if constexpr (T == graph_type::DIRECTED) {
    return vertex_outdegree(graph, vertex_id) +
           vertex_indegree(graph, vertex_id);
  } else {
    return vertex_outdegree(graph, vertex_id);
  }
}

}  // namespace graaf::properties
