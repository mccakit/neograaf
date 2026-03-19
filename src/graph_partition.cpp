module;
export module graaf:graph;
import std;
import :types;
import :edge;

export namespace graaf {

// ---------------------------------------------------------------------------
// detail
// ---------------------------------------------------------------------------

namespace detail {

inline std::pair<vertex_id_t, vertex_id_t> make_sorted_pair(
    vertex_id_t vertex_id_lhs, vertex_id_t vertex_id_rhs) {
  if (vertex_id_lhs < vertex_id_rhs) {
    return std::make_pair(vertex_id_lhs, vertex_id_rhs);
  }
  return std::make_pair(vertex_id_rhs, vertex_id_lhs);
}

}  // namespace detail

// ---------------------------------------------------------------------------
// graph
// ---------------------------------------------------------------------------

template <typename VERTEX_T, typename EDGE_T, graph_type GRAPH_TYPE_V>
class graph {
 public:
  using vertex_t              = VERTEX_T;
  using edge_t                = EDGE_T;
  using vertices_t            = std::unordered_set<vertex_id_t>;
  using vertex_id_to_vertex_t = std::unordered_map<vertex_id_t, VERTEX_T>;
  using edge_id_to_edge_t     = std::unordered_map<edge_id_t, edge_t, edge_id_hash>;

  [[nodiscard]] constexpr bool is_directed() const noexcept {
    return GRAPH_TYPE_V == graph_type::DIRECTED;
  }

  [[nodiscard]] constexpr bool is_undirected() const noexcept {
    return GRAPH_TYPE_V == graph_type::UNDIRECTED;
  }

  [[nodiscard]] std::size_t vertex_count() const noexcept {
    return vertices_.size();
  }

  [[nodiscard]] std::size_t edge_count() const noexcept {
    return edges_.size();
  }

  [[nodiscard]] const vertex_id_to_vertex_t& get_vertices() const noexcept {
    return vertices_;
  }

  [[nodiscard]] const edge_id_to_edge_t& get_edges() const noexcept {
    return edges_;
  }

  [[nodiscard]] bool has_vertex(vertex_id_t vertex_id) const noexcept {
    return vertices_.contains(vertex_id);
  }

  [[nodiscard]] bool has_edge(vertex_id_t vertex_id_lhs,
                              vertex_id_t vertex_id_rhs) const noexcept {
    using enum graph_type;
    if constexpr (GRAPH_TYPE_V == DIRECTED) {
      return edges_.contains({vertex_id_lhs, vertex_id_rhs});
    } else {
      return edges_.contains(
          detail::make_sorted_pair(vertex_id_lhs, vertex_id_rhs));
    }
  }

  [[nodiscard]] vertex_t& get_vertex(vertex_id_t vertex_id) {
    return const_cast<vertex_t&>(
        const_cast<const graph*>(this)->get_vertex(vertex_id));
  }

  [[nodiscard]] const vertex_t& get_vertex(vertex_id_t vertex_id) const {
    if (!has_vertex(vertex_id)) {
      throw std::invalid_argument{"Vertex with ID [" +
                                  std::to_string(vertex_id) +
                                  "] not found in graph."};
    }
    return vertices_.at(vertex_id);
  }

  [[nodiscard]] edge_t& get_edge(vertex_id_t vertex_id_lhs,
                                 vertex_id_t vertex_id_rhs) {
    return const_cast<edge_t&>(
        const_cast<const graph*>(this)->get_edge(vertex_id_lhs, vertex_id_rhs));
  }

  [[nodiscard]] const edge_t& get_edge(vertex_id_t vertex_id_lhs,
                                       vertex_id_t vertex_id_rhs) const {
    if (!has_edge(vertex_id_lhs, vertex_id_rhs)) {
      throw std::invalid_argument{"No edge found between vertices [" +
                                  std::to_string(vertex_id_lhs) + "] -> [" +
                                  std::to_string(vertex_id_rhs) + "]."};
    }
    using enum graph_type;
    if constexpr (GRAPH_TYPE_V == DIRECTED) {
      return edges_.at({vertex_id_lhs, vertex_id_rhs});
    } else {
      return edges_.at(
          detail::make_sorted_pair(vertex_id_lhs, vertex_id_rhs));
    }
  }

  [[nodiscard]] edge_t& get_edge(const edge_id_t& edge_id) {
    const auto [lhs, rhs]{edge_id};
    return get_edge(lhs, rhs);
  }

  [[nodiscard]] const edge_t& get_edge(const edge_id_t& edge_id) const {
    const auto [lhs, rhs]{edge_id};
    return get_edge(lhs, rhs);
  }

  [[nodiscard]] const vertices_t& get_neighbors(vertex_id_t vertex_id) const {
    static const vertices_t empty{};
    if (!adjacency_list_.contains(vertex_id)) return empty;
    return adjacency_list_.at(vertex_id);
  }

  [[nodiscard]] vertex_id_t add_vertex(auto&& vertex) {
    while (has_vertex(vertex_id_supplier_)) ++vertex_id_supplier_;
    const auto id{vertex_id_supplier_};
    vertices_.emplace(id, std::forward<decltype(vertex)>(vertex));
    return id;
  }

  vertex_id_t add_vertex(auto&& vertex, vertex_id_t id) {
    if (has_vertex(id)) {
      throw std::invalid_argument{"Vertex already exists at ID [" +
                                  std::to_string(id) + "]"};
    }
    vertices_.emplace(id, std::forward<decltype(vertex)>(vertex));
    return id;
  }

  void remove_vertex(vertex_id_t vertex_id) {
    if (adjacency_list_.contains(vertex_id)) {
      for (const auto& target : adjacency_list_.at(vertex_id)) {
        edges_.erase({vertex_id, target});
      }
    }
    adjacency_list_.erase(vertex_id);
    vertices_.erase(vertex_id);

    for (auto& [source, neighbors] : adjacency_list_) {
      neighbors.erase(vertex_id);
      edges_.erase({source, vertex_id});
    }
  }

  void add_edge(vertex_id_t vertex_id_lhs, vertex_id_t vertex_id_rhs,
                auto&& edge) {
    if (!has_vertex(vertex_id_lhs) || !has_vertex(vertex_id_rhs)) {
      throw std::invalid_argument{
          "Vertices with ID [" + std::to_string(vertex_id_lhs) + "] and [" +
          std::to_string(vertex_id_rhs) + "] not found in graph."};
    }
    using enum graph_type;
    if constexpr (GRAPH_TYPE_V == DIRECTED) {
      adjacency_list_[vertex_id_lhs].insert(vertex_id_rhs);
      edges_.emplace(std::make_pair(vertex_id_lhs, vertex_id_rhs),
                     std::forward<decltype(edge)>(edge));
    } else {
      adjacency_list_[vertex_id_lhs].insert(vertex_id_rhs);
      adjacency_list_[vertex_id_rhs].insert(vertex_id_lhs);
      edges_.emplace(detail::make_sorted_pair(vertex_id_lhs, vertex_id_rhs),
                     std::forward<decltype(edge)>(edge));
    }
  }

  void remove_edge(vertex_id_t vertex_id_lhs, vertex_id_t vertex_id_rhs) {
    using enum graph_type;
    if constexpr (GRAPH_TYPE_V == DIRECTED) {
      adjacency_list_.at(vertex_id_lhs).erase(vertex_id_rhs);
      edges_.erase(std::make_pair(vertex_id_lhs, vertex_id_rhs));
    } else {
      adjacency_list_.at(vertex_id_lhs).erase(vertex_id_rhs);
      adjacency_list_.at(vertex_id_rhs).erase(vertex_id_lhs);
      edges_.erase(detail::make_sorted_pair(vertex_id_lhs, vertex_id_rhs));
    }
  }

 private:
  std::unordered_map<vertex_id_t, vertices_t> adjacency_list_{};
  vertex_id_to_vertex_t                       vertices_{};
  edge_id_to_edge_t                           edges_{};
  std::size_t                                 vertex_id_supplier_{0};
};

// ---------------------------------------------------------------------------
// Convenience aliases
// ---------------------------------------------------------------------------

template <typename VERTEX_T, typename EDGE_T>
using directed_graph = graph<VERTEX_T, EDGE_T, graph_type::DIRECTED>;

template <typename VERTEX_T, typename EDGE_T>
using undirected_graph = graph<VERTEX_T, EDGE_T, graph_type::UNDIRECTED>;

}  // namespace graaf
