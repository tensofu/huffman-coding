#include <algorithm>
#include <cstdint>
#include <memory>
#include <string> // For std::string

/* Huffman Tree Stuff
  Leaf nodes should have a character and frequency count, with references to
*/

namespace Huffman {
struct Node {
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;
  uint32_t freq;
  char sym;

  // Constructor for a Leaf Node
  Node(char c, uint32_t i) : sym(c), freq(i), left(nullptr), right(nullptr) {};

  // Constructor for an Internal Node
  Node(uint32_t i, Node l, Node r)
      : sym('\0'), freq(i), left(std::make_unique<Node>(std::move(l))),
        right(std::make_unique<Node>(std::move(r))) {}

  // Move Constructor
  Node(Node &&other) noexcept
      : sym(other.sym), freq(other.freq), left(std::move(other.left)),
        right(std::move(other.right)) {}

  // Move Assignment Operator
  Node &operator=(Node &&other) noexcept {
    // Check for self-assignment (e.g., n = std::move(n))
    if (this != &other) {
      // Move all the data
      sym = other.sym;
      freq = other.freq;
      left = std::move(other.left);
      right = std::move(other.right);
    }
    return *this;
  }

  // Comparison Operators
  friend bool operator<(const Node &lhs, const Node &rhs) {
    return lhs.freq < rhs.freq;
  }
  friend bool operator>(const Node &lhs, const Node &rhs) {
    return lhs.freq > rhs.freq;
  }
  friend bool operator==(const Node &lhs, const Node &rhs) {
    return lhs.freq == rhs.freq;
  }
  friend bool operator!=(const Node &lhs, const Node &rhs) {
    return lhs.freq != rhs.freq;
  }

  // ostream operator
  friend std::ostream &operator<<(std::ostream &os, const Node &n) {
    os << "Huffman::Node[Char: " << n.sym << ", Freq: " << n.freq << "]";
    return os;
  };

  void print() {
    std::cout << "Huffman::Node[Char: " << (*this).sym
              << ", Freq: " << (*this).freq << "]" << std::endl;
  }

  // Checks if leaf
  bool is_leaf() const { return left == nullptr && right == nullptr; }
};
class Tree {
private:
  std::unique_ptr<Node> root;

  void
  build_codes_helper(const Node *node, std::string current_code,
                     std::unordered_map<char, std::string> &codes_map) const {
    // Safety check 
    if (node == nullptr) {
      return;
    }

    // Base case: We are at a leaf node
    if (node->is_leaf()) {
      // Special case: If the tree has only one node (e.g., file "aaaaa")
      // assign it a code of '0'.
      if (current_code.empty()) {
        codes_map[node->sym] = "0";
      } else {
        codes_map[node->sym] = current_code;
      }
      return; // Stop recursion for this path
    }

    // Go left, appending '0'
    build_codes_helper(node->left.get(), current_code + '0', codes_map);

    // Go right, appending '1'
    build_codes_helper(node->right.get(), current_code + '1', codes_map);
  }

public:
  // Builds tree of nodes
  Tree(const std::unordered_map<char, uint32_t> &ft) {
    std::vector<Node> pq;
    pq.reserve(256);

    // In case table is empty
    if (ft.empty()) {
      root = nullptr;
      return;
    }

    // Pushes stuff to the pq
    for (const auto &[k, v] : ft) {
      pq.push_back(Node(k, v));
      std::push_heap(pq.begin(), pq.end(), std::greater<Node>{}); // Re-heap
    }

    while (pq.size() > 1) {
      // 1. Get the first min node
      std::pop_heap(pq.begin(), pq.end(),
                    std::greater<Node>{}); // Moves min to back
      Node left = std::move(pq.back());    // Move from back
      pq.pop_back();                       // Remove from vector

      // 2. Get the second min node
      std::pop_heap(pq.begin(), pq.end(), std::greater<Node>{});
      Node right = std::move(pq.back());
      pq.pop_back();

      // 3. Create the new internal node by moving the nodes
      Node internal(left.freq + right.freq, std::move(left), std::move(right));

      // 4. Push the new internal node back onto the heap
      pq.push_back(std::move(internal));
      std::push_heap(pq.begin(), pq.end(), std::greater<Node>{});
    }

    if (!pq.empty()) {
      root = std::make_unique<Node>(std::move(pq.front()));
    }
  };

  std::unordered_map<char, std::string> generate_codes() const {
    std::unordered_map<char, std::string> codes_map;

    // Start the recursive helper, passing it the map
    build_codes_helper(root.get(), "", codes_map);

    return codes_map;
  }
};
}; // namespace Huffman
