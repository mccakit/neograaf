module;
export module graaf:tree;
import std;
import :types;

export namespace graaf {

/**
 * @brief A generic tree structure.
 * * @tparam VERTEX_T The type of the vertex.
 * @tparam EDGE_T The type of the edge.
 */
template <typename VERTEX_T, typename EDGE_T>
class tree {
public:
    // Forward declarations for nested types
    struct tree_node;
    struct child_link;

    /**
     * @brief Constructs a tree with a root value.
     */
    explicit tree(VERTEX_T root_val)
        : root_{std::make_unique<tree_node>(std::move(root_val), nullptr,
                                            std::vector<child_link>{})} {}

    [[nodiscard]] tree_node* root() { return root_.get(); }
    [[nodiscard]] const tree_node* root() const { return root_.get(); }

    /**
     * @brief Represents a node in the tree.
     */
    struct tree_node {
        VERTEX_T value{};
        tree_node* parent{}; // raw pointer to break cyclic dependency
        std::vector<child_link> children{};

        /**
         * @brief Adds a child to this node.
         * Implementation merged from tree.tpp logic.
         */
        [[nodiscard]] tree_node* add_child(EDGE_T edge_val, VERTEX_T child_val) {
            // Create the new node, passing 'this' as the parent
            auto new_node = std::make_unique<tree_node>(std::move(child_val), this, std::vector<child_link>{});
            tree_node* raw_ptr = new_node.get();

            // Link it via a child_link
            children.push_back({std::move(edge_val), std::move(new_node)});

            return raw_ptr;
        }
    };

    /**
     * @brief Represents a link to a child node with an associated edge value.
     */
    struct child_link {
        EDGE_T value{};
        std::unique_ptr<tree_node> child{};
    };

private:
    std::unique_ptr<tree_node> root_{};
};

} // namespace graaf
