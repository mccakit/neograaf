module;

export module graaf:tarjan;

import std;
import :graph;
import :strongly_connected_components_common;

export namespace graaf::algorithm {

template <typename V, typename E>
[[nodiscard]] sccs_t tarjans_strongly_connected_components(
    const graph<V, E, graph_type::DIRECTED>& graph) {
  sccs_t sccs{};
  std::stack<vertex_id_t> stack{};
  std::unordered_map<vertex_id_t, std::size_t> indices{};
  std::unordered_map<vertex_id_t, std::size_t> low_links{};
  std::unordered_map<vertex_id_t, bool> on_stack{};
  std::size_t index_counter{0};

  std::function<void(vertex_id_t)> strong_connect;
  strong_connect = [&](vertex_id_t vertex) {
    indices[vertex]   = index_counter;
    low_links[vertex] = index_counter;
    ++index_counter;

    stack.push(vertex);
    on_stack[vertex] = true;

    for (const auto neighbor : graph.get_neighbors(vertex)) {
      if (!indices.contains(neighbor)) {
        strong_connect(neighbor);
        low_links[vertex] = std::min(low_links[vertex], low_links[neighbor]);
      } else if (on_stack[neighbor]) {
        low_links[vertex] = std::min(low_links[vertex], indices[neighbor]);
      }
    }

    if (low_links[vertex] == indices[vertex]) {
      std::vector<vertex_id_t> scc{};
      vertex_id_t top{};
      do {
        top = stack.top();
        stack.pop();
        on_stack[top] = false;
        scc.push_back(top);
      } while (top != vertex);
      sccs.push_back(std::move(scc));
    }
  };

  for (const auto& [vertex_id, _] : graph.get_vertices()) {
    if (!indices.contains(vertex_id)) {
      strong_connect(vertex_id);
    }
  }

  return sccs;
}

}  // namespace graaf::algorithm
