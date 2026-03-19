module;

export module graaf:graph_traversal_common;

import :types;

export namespace graaf::algorithm::detail {

struct noop_callback {
  void operator()(const edge_id_t& /*edge*/) const {}
};

struct exhaustive_search_strategy {
  [[nodiscard]] bool operator()(const vertex_id_t /*vertex*/) const {
    return false;
  }
};

}  // namespace graaf::algorithm::detail
