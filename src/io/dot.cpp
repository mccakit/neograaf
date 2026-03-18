module;
export module graaf:io;
import std;
import :types;
import :edge;
import :graph;

export namespace graaf::io {

namespace detail {

template <typename T>
concept string_convertible = requires(T element) { std::to_string(element); };

template <typename T>
  requires string_convertible<T>
const auto default_vertex_writer{
    [](vertex_id_t vertex_id, const T& vertex) -> std::string {
      return "label=\"" + std::to_string(vertex_id) + ": " +
             std::to_string(vertex) + "\"";
    }};

const auto default_edge_writer{
    [](const edge_id_t& /*edge_id*/, const auto& edge) -> std::string {
      return "label=\"" + std::to_string(get_weight(edge)) + "\"";
    }};

constexpr const char* graph_type_to_string(const graph_type& type) {
  switch (type) {
    using enum graph_type;
    case DIRECTED:
      return "digraph";
    case UNDIRECTED:
      return "graph";
    default:
      std::abort();
  }
}

constexpr const char* graph_type_to_edge_specifier(const graph_type& type) {
  switch (type) {
    using enum graph_type;
    case DIRECTED:
      return "->";
    case UNDIRECTED:
      return "--";
    default:
      std::abort();
  }
}

}  // namespace detail

template <typename V, typename E, graph_type T,
          typename VERTEX_WRITER_T = decltype(detail::default_vertex_writer<V>),
          typename EDGE_WRITER_T = decltype(detail::default_edge_writer)>
  requires std::is_invocable_r_v<std::string, const VERTEX_WRITER_T&,
                                 vertex_id_t, const V&> &&
           std::is_invocable_r_v<std::string, const EDGE_WRITER_T&,
                                 const graaf::edge_id_t&,
                                 const typename graph<V, E, T>::edge_t&>
void to_dot(
    const graph<V, E, T>& graph, const std::filesystem::path& path,
    const VERTEX_WRITER_T& vertex_writer = detail::default_vertex_writer<V>,
    const EDGE_WRITER_T& edge_writer = detail::default_edge_writer) {
  std::ofstream dot_file{path};
  const auto append_line{
      [&dot_file](const std::string& line) { dot_file << line << std::endl; }};

  append_line(std::string(detail::graph_type_to_string(T)) + " {");

  for (const auto& [vertex_id, vertex] : graph.get_vertices()) {
    append_line("\t" + std::to_string(vertex_id) + " [" +
                vertex_writer(vertex_id, vertex) + "];");
  }

  const auto edge_specifier{detail::graph_type_to_edge_specifier(T)};
  for (const auto& [edge_id, edge] : graph.get_edges()) {
    const auto [source_id, target_id]{edge_id};
    append_line("\t" + std::to_string(source_id) + " " + edge_specifier + " " +
                std::to_string(target_id) + " [" + edge_writer(edge_id, edge) +
                "];");
  }

  append_line("}");
}

}  // namespace graaf::io
