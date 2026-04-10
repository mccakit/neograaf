module;

export module graaf:clique_detection;

import std;
import :graph;

export namespace graaf::algorithm {

namespace detail {

using clique_collection_t = std::vector<std::vector<vertex_id_t>>;
using vertex_set_t = std::unordered_set<vertex_id_t>;

vertex_set_t do_get_intersection(const vertex_set_t &lhs,
                                 const vertex_set_t &rhs) {
  vertex_set_t result{};
  for (const auto &v : lhs) {
    if (rhs.contains(v))
      result.insert(v);
  }
  return result;
}

vertex_set_t set_union(const vertex_set_t &lhs, const vertex_set_t &rhs) {
  vertex_set_t result = lhs;
  for (const auto &v : rhs)
    result.insert(v);
  return result;
}

vertex_set_t set_difference(const vertex_set_t &lhs, const vertex_set_t &rhs) {
  vertex_set_t result = lhs;
  for (const auto &v : rhs)
    result.erase(v);
  return result;
}

template <typename V, typename E>
void do_bron_kerbosch_maximal_clique(
    std::vector<vertex_id_t> &clique, vertex_set_t &vertices,
    vertex_set_t &excluded_vertices, clique_collection_t &maximal_cliques,
    const graph<V, E, graph_type::UNDIRECTED> &graph) {
  if (vertices.empty() && excluded_vertices.empty()) {
    maximal_cliques.push_back(clique);
    return;
  }

  auto union_set = set_union(vertices, excluded_vertices);

  const auto pivot =
      *std::ranges::max_element(union_set, {}, [&graph](vertex_id_t id) {
        return graph.get_neighbors(id).size();
      });

  auto to_process = set_difference(vertices, graph.get_neighbors(pivot));

  for (const auto &vertex : to_process) {
    const auto neighbors = graph.get_neighbors(vertex);

    auto vertices_intersection = do_get_intersection(vertices, neighbors);
    auto excluded_intersection =
        do_get_intersection(excluded_vertices, neighbors);

    clique.push_back(vertex);
    do_bron_kerbosch_maximal_clique(clique, vertices_intersection,
                                    excluded_intersection, maximal_cliques,
                                    graph);
    clique.pop_back();

    vertices.erase(vertex);
    excluded_vertices.insert(vertex);
  }
}

} // namespace detail

// ---------------------------------------------------------------------------
// bron_kerbosch
// ---------------------------------------------------------------------------

template <typename V, typename E>
[[nodiscard]] std::vector<std::vector<vertex_id_t>>
bron_kerbosch(const graph<V, E, graph_type::UNDIRECTED> &graph) {
  detail::clique_collection_t maximal_cliques{};
  std::vector<vertex_id_t> clique{};
  detail::vertex_set_t vertices{};
  detail::vertex_set_t excluded{};

  for (const auto &[vertex_id, _] : graph.get_vertices()) {
    vertices.insert(vertex_id);
  }

  detail::do_bron_kerbosch_maximal_clique(clique, vertices, excluded,
                                          maximal_cliques, graph);
  return maximal_cliques;
}

} // namespace graaf::algorithm
