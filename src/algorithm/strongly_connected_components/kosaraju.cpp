module;

export module graaf:kosaraju;

import std;
import :graph;
import :strongly_connected_components_common;
import :algorithm_utils;

export namespace graaf::algorithm {

namespace detail {

template <typename V, typename E>
void do_visit_vertex(vertex_id_t vertex_id,
                     const directed_graph<V, E>& graph,
                     std::stack<vertex_id_t>& stack,
                     std::unordered_set<vertex_id_t>& seen) {
  if (seen.contains(vertex_id)) return;
  seen.insert(vertex_id);
  for (const auto neighbour : graph.get_neighbors(vertex_id)) {
    do_visit_vertex(neighbour, graph, stack, seen);
  }
  stack.push(vertex_id);
}

template <typename V, typename E>
void collect_scc(vertex_id_t vertex,
                 const directed_graph<V, E>& transposed,
                 std::vector<vertex_id_t>& scc,
                 std::unordered_set<vertex_id_t>& seen) {
  if (seen.contains(vertex)) return;
  seen.insert(vertex);
  scc.push_back(vertex);
  for (const auto neighbour : transposed.get_neighbors(vertex)) {
    collect_scc(neighbour, transposed, scc, seen);
  }
}

}  // namespace detail

template <typename V, typename E>
[[nodiscard]] sccs_t kosarajus_strongly_connected_components(
    const directed_graph<V, E>& graph) {
  sccs_t sccs{};

  if (graph.get_vertices().empty()) return sccs;

  // Pass 1: fill stack by finish time
  std::stack<vertex_id_t> stack{};
  std::unordered_set<vertex_id_t> seen{};

  for (const auto& [vertex_id, _] : graph.get_vertices()) {
    if (!seen.contains(vertex_id)) {
      detail::do_visit_vertex(vertex_id, graph, stack, seen);
    }
  }

  // Pass 2: collect SCCs from transposed graph in finish-time order
  const auto transposed{get_transposed_graph(graph)};
  seen.clear();

  while (!stack.empty()) {
    const auto top{stack.top()};
    stack.pop();

    if (!seen.contains(top)) {
      std::vector<vertex_id_t> scc{};
      detail::collect_scc(top, transposed, scc, seen);
      sccs.push_back(std::move(scc));
    }
  }

  return sccs;
}

}  // namespace graaf::algorithm
