module;

export module graaf:breadth_first_search;

import std;
import :graph;
import :graph_traversal_common;

export namespace graaf::algorithm {

template <
    typename V, typename E, graph_type T,
    typename EDGE_CALLBACK_T = detail::noop_callback,
    typename SEARCH_TERMINATION_STRATEGY_T = detail::exhaustive_search_strategy>
  requires std::invocable<EDGE_CALLBACK_T&, edge_id_t&> &&
           std::is_invocable_r_v<bool, SEARCH_TERMINATION_STRATEGY_T&,
                                 vertex_id_t>
void breadth_first_traverse(
    const graph<V, E, T>& graph,
    vertex_id_t start_vertex,
    const EDGE_CALLBACK_T& edge_callback = EDGE_CALLBACK_T{},
    const SEARCH_TERMINATION_STRATEGY_T& search_termination_strategy =
        SEARCH_TERMINATION_STRATEGY_T{}) {
  std::unordered_set<vertex_id_t> seen_vertices{};
  std::queue<vertex_id_t> to_explore{};

  to_explore.push(start_vertex);

  while (!to_explore.empty()) {
    const auto current{to_explore.front()};
    to_explore.pop();

    if (search_termination_strategy(current)) return;

    seen_vertices.insert(current);

    for (const auto neighbor : graph.get_neighbors(current)) {
      if (!seen_vertices.contains(neighbor)) {
        edge_callback(edge_id_t{current, neighbor});
        to_explore.push(neighbor);
        seen_vertices.insert(neighbor);  // mark on enqueue, not on visit
      }
    }
  }
}

}  // namespace graaf::algorithm
