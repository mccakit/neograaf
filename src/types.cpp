module;
import std;
export module graaf:types;

/**
 * Based on boost::hash_combine. Since Boost is licensed under the Boost
 * Software License, we include a copy of the license here.
 * * Boost Software License - Version 1.0 - August 17th, 2003
 * ... (License text omitted for brevity in code block, but preserved in your
 * source)
 */

export namespace graaf {
// ---------------------------------------------------------------------------
// graph_type
// ---------------------------------------------------------------------------

enum class graph_type { DIRECTED, UNDIRECTED };
using vertex_id_t = std::size_t;
using edge_id_t = std::pair<vertex_id_t, vertex_id_t>;

/**
 * Utility for combining hashes.
 * Note: While this is used internally for edge_id_hash,
 * we export it as it's a useful general utility for graph types.
 */
template <class T> void hash_combine(std::size_t &seed, const T &v) {
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct edge_id_hash {
  [[nodiscard]] std::size_t operator()(const edge_id_t &key) const {
    std::size_t seed = 0;
    hash_combine(seed, key.first);
    hash_combine(seed, key.second);
    return seed;
  }
};

} // namespace graaf
