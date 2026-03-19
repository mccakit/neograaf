module;

export module graaf:depth_first_search;

import std;
import :graph;
import :graph_traversal_common;

export namespace graaf::algorithm {

namespace detail {

template <typename V, typename E, graph_type T,
          typename EDGE_CALLBACK_T,
          typename SEARCH_TERMINATION_STRATEGY_T>
bool do_dfs(
    const graph<V, E, T>& graph,
    std::unordered_set<vertex_id_t>& seen_vertices,
    vertex_id_t current,
    const EDGE_CALLBACK_T& edge_callback,
    const SEARCH_TERMINATION_STRATEGY_T& search_termination_strategy) {
  seen_vertices.insert(current);

  if (search_termination_strategy(current)) return false;

  for (const auto neighbor : graph.get_neighbors(current)) {
    if (!seen_vertices.contains(neighbor)) {
      edge_callback(edge_id_t{current, neighbor});
      if (!do_dfs(graph, seen_vertices, neighbor, edge_callback,
                  search_termination_strategy)) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace detail

template <
    typename V, typename E, graph_type T,
    typename EDGE_CALLBACK_T = detail::noop_callback,
    typename SEARCH_TERMINATION_STRATEGY_T = detail::exhaustive_search_strategy>
  requires std::invocable<EDGE_CALLBACK_T&, edge_id_t&> &&
           std::is_invocable_r_v<bool, SEARCH_TERMINATION_STRATEGY_T&,
                                 vertex_id_t>
void depth_first_traverse(
    const graph<V, E, T>& graph,
    vertex_id_t start_vertex,
    const EDGE_CALLBACK_T& edge_callback = EDGE_CALLBACK_T{},
    const SEARCH_TERMINATION_STRATEGY_T& search_termination_strategy =
        SEARCH_TERMINATION_STRATEGY_T{}) {
  std::unordered_set<vertex_id_t> seen_vertices{};
  detail::do_dfs(graph, seen_vertices, start_vertex, edge_callback,
                 search_termination_strategy);
}

}  // namespace graaf::algorithm
