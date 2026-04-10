module;

export module graaf:kruskal;

import std;
import :graph;
import :edge;

export namespace graaf::algorithm {

namespace detail {

void do_make_set(vertex_id_t v,
                 std::unordered_map<vertex_id_t, vertex_id_t> &parent,
                 std::unordered_map<vertex_id_t, vertex_id_t> &rank) {
  parent[v] = v;
  rank[v] = 0;
}

vertex_id_t do_find_set(vertex_id_t vertex,
                        std::unordered_map<vertex_id_t, vertex_id_t> &parent) {
  if (vertex == parent[vertex])
    return vertex;
  return parent[vertex] = do_find_set(parent[vertex], parent);
}

void do_merge_sets(vertex_id_t a, vertex_id_t b,
                   std::unordered_map<vertex_id_t, vertex_id_t> &parent,
                   std::unordered_map<vertex_id_t, vertex_id_t> &rank) {
  a = do_find_set(a, parent);
  b = do_find_set(b, parent);

  if (a == b)
    return;

  if (rank[a] < rank[b])
    std::swap(a, b);
  parent[b] = a;
  if (rank[a] == rank[b])
    ++rank[a];
}

template <typename E> struct edge_to_process {
  vertex_id_t vertex_a, vertex_b;
  E weight;

  edge_to_process(vertex_id_t a, vertex_id_t b, E w)
      : vertex_a{a}, vertex_b{b}, weight{w} {}

  [[nodiscard]] bool operator<(const edge_to_process &other) const {
    if (weight != other.weight)
      return weight < other.weight;
    if (vertex_a != other.vertex_a)
      return vertex_a < other.vertex_a;
    return vertex_b < other.vertex_b;
  }
};

} // namespace detail

template <typename V, typename E>
[[nodiscard]] std::vector<edge_id_t> kruskal_minimum_spanning_tree(
    const graph<V, E, graph_type::UNDIRECTED> &graph) {
  std::unordered_map<vertex_id_t, vertex_id_t> parent, rank;
  std::vector<detail::edge_to_process<E>> edges_to_process{};
  std::vector<edge_id_t> mst{};

  for (const auto &[vertex_id, _] : graph.get_vertices()) {
    detail::do_make_set(vertex_id, parent, rank);
  }

  for (const auto &[edge_id, edge] : graph.get_edges()) {
    edges_to_process.emplace_back(edge_id.first, edge_id.second,
                                  get_weight(edge));
  }

  std::ranges::sort(edges_to_process, std::less{});

  for (const auto &e : edges_to_process) {
    if (detail::do_find_set(e.vertex_a, parent) !=
        detail::do_find_set(e.vertex_b, parent)) {
      mst.push_back({e.vertex_a, e.vertex_b});
      detail::do_merge_sets(e.vertex_a, e.vertex_b, parent, rank);
    }
    if (mst.size() == graph.vertex_count() - 1)
      return mst;
  }

  return mst;
}

} // namespace graaf::algorithm
