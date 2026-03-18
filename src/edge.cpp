export module graaf:edge;

import std;
import :types;

export namespace graaf {

// ---------------------------------------------------------------------------
// weighted_edge interface
// ---------------------------------------------------------------------------

template <typename WEIGHT_T = int>
class weighted_edge {
 public:
  using weight_t = WEIGHT_T;

  virtual ~weighted_edge() = default;

  [[nodiscard]] virtual WEIGHT_T get_weight() const noexcept = 0;
};

// ---------------------------------------------------------------------------
// Concept
// ---------------------------------------------------------------------------

template <typename DERIVED>
concept derived_from_weighted_edge =
    std::is_base_of_v<weighted_edge<typename DERIVED::weight_t>, DERIVED>;

// ---------------------------------------------------------------------------
// get_weight overload set
//
// Overload 1: types derived from weighted_edge  → call virtual get_weight()
// Overload 2: arithmetic primitives             → identity (the value is the weight)
// Overload 3: everything else                   → unit weight (1)
// ---------------------------------------------------------------------------

template <typename WEIGHTED_EDGE_T>
  requires derived_from_weighted_edge<WEIGHTED_EDGE_T>
[[nodiscard]] auto get_weight(const WEIGHTED_EDGE_T& edge) {
  return edge.get_weight();
}

template <typename EDGE_T>
  requires std::is_arithmetic_v<EDGE_T>
[[nodiscard]] EDGE_T get_weight(const EDGE_T& edge) {
  return edge;
}

template <typename EDGE_T>
[[nodiscard]] int get_weight(const EDGE_T& /*edge*/) {
  // By default, an unweighted edge has unit weight
  return 1;
}

}  // namespace graaf
