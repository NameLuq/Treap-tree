#include <iostream>
#include <vector>
#include <random>
#include <memory>
#include <set>
#include <chrono>

namespace Treap
{

template <typename T>
class Treap
{
  private:
    struct Node
    {
        Node *left, *right;
        uint_fast64_t priority;
        T key;

        static inline std::random_device rd;
        static inline std::mt19937 e{rd()};
        static inline std::uniform_int_distribution<uint_fast64_t> dist;

        Node(T key_) : left(nullptr), right(nullptr),
                       key(key_), priority(dist(e)) {}
    };

    Node *root;
    std::vector<std::unique_ptr<Node>> nodes;

    // utility operations: split and merge
    void split(Node *tree, const T &val, Node *&left, Node *&right)
    {
        if (!tree)
        {
            left = right = nullptr;
            return;
        }
        if (val < tree->key)
        {
            split(tree->left, val, left, tree->left);
            right = tree;
            return;
        }
        split(tree->right, val, tree->right, right);
        left = tree;
        return;
    }

    void merge(Node *&tree, Node *left, Node *right)
    {
        if (!left || !right)
        {
            tree = left ? left : right;
            return;
        }
        if (left->priority > right->priority)
        {
            merge(left->right, left->right, right);
            tree = left;
            return;
        }
        merge(right->left, left, right->left);
        tree = right;
        return;
    }

    // private stuff
    void insert(Node *&tree, Node *elem)
    {
        if (!tree)
        {
            tree = elem;
            return;
        }

        if (elem->key == tree->key)
            return;

        if (elem->priority > tree->priority)
        {
            split(tree, elem->key, elem->left, elem->right);
            tree = elem;
            return;
        }

        insert(elem->key < tree->key ? tree->left : tree->right, elem);
    }

    void remove(Node *&tree, const T &val)
    {
        if (!tree)
            return;

        if (tree->key == val)
        {
            merge(tree, tree->left, tree->right);
            return;
        }
        remove(val < tree->key ? tree->left : tree->right, val);
    }

    void print(Node *node) const
    {
        if (!node)
            return;

        print(node->left);
        std::cout << node->key << ' ' << node->priority << std::endl;
        print(node->right);
    }

    bool search(Node *node, const T &val) const
    {
        if (node->key == val)
            return true;

        if ((val < node->key) && node->left)
            return search(node->left, val);
        if ((val > node->key) && node->right)
            return search(node->right, val);

        return false;
    }

  public: // public API
    Treap() : root(nullptr) {}

    void insert(const T &val)
    {
        insert(root, (nodes.emplace_back(new Node(val)), nodes.back().get()));
    }

    void remove(const T &val)
    {
        remove(root, val);
    }

    bool search(const T &val) const
    {
        return search(root, val);
    }

    void print() const
    {
        print(root);
    }
};
} // namespace Treap

int main()
{
    const int N = 1'000'000;

    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<uint_fast64_t> dist;

    Treap::Treap<uint_fast64_t> m_treap;

    for (int i = 0; i < N; ++i)
        m_treap.insert(dist(e));
}
