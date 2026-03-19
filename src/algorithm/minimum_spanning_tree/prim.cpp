module;

export module graaf:prim;

import std;
import :graph;
import :edge;

export namespace graaf::algorithm {

namespace detail {

template <typename GRAPH_T>
[[nodiscard]] std::vector<edge_id_t> find_candidate_edges(
    const GRAPH_T& graph,
    const std::unordered_set<vertex_id_t>& fringe_vertices) {
  std::vector<edge_id_t> candidates{};

  for (const auto fringe_vertex : fringe_vertices) {
    for (const auto neighbor : graph.get_neighbors(fringe_vertex)) {
      if (!fringe_vertices.contains(neighbor)) {
        candidates.emplace_back(fringe_vertex, neighbor);
      }
    }
  }

  return candidates;
}

}  // namespace detail

template <typename V, typename E>
[[nodiscard]] std::optional<std::vector<edge_id_t>> prim_minimum_spanning_tree(
    const graph<V, E, graph_type::UNDIRECTED>& graph,
    vertex_id_t start_vertex) {
  std::vector<edge_id_t> mst{};
  mst.reserve(graph.edge_count());

  std::unordered_set<vertex_id_t> fringe{start_vertex};

  while (fringe.size() < graph.vertex_count()) {
    const auto candidates{detail::find_candidate_edges(graph, fringe)};

    if (candidates.empty()) return std::nullopt;

    const edge_id_t best{*std::ranges::min_element(
        candidates,
        [&graph](const edge_id_t& lhs, const edge_id_t& rhs) {
          return get_weight(graph.get_edge(lhs)) <
                 get_weight(graph.get_edge(rhs));
        })};

    mst.emplace_back(best);
    fringe.insert(best.second);
  }

  return mst;
}

}  // namespace graaf::algorithm
