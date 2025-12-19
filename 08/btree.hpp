#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <type_traits>

template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
>
class bst {
private:
    struct TreeNode {
        std::pair<const Key, T> value;
        TreeNode *left;
        TreeNode *right;
        TreeNode *parent;

        TreeNode(const std::pair<const Key, T>& v, TreeNode *p = nullptr)
            : value(v), left(nullptr), right(nullptr), parent(p) {}

        TreeNode(std::pair<const Key, T>&& v, TreeNode *p = nullptr)
            : value(std::move(v)), left(nullptr), right(nullptr), parent(p) {}
    };

    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<TreeNode>;
    using NodeTraits = std::allocator_traits<NodeAlloc>;

    TreeNode *root;
    size_t size_;
    Compare comp;
    NodeAlloc alloc;

    TreeNode* createNode(const std::pair<const Key, T>& val, TreeNode *parent = nullptr);
    TreeNode* createNode(std::pair<const Key, T>&& val, TreeNode *parent = nullptr);
    void destroyNode(TreeNode *node);
    void clearRec(TreeNode *node);
    static TreeNode* minNode(TreeNode *node);
    static const TreeNode* minNode(const TreeNode *node);
    static TreeNode* maxNode(TreeNode *node);
    static const TreeNode* maxNode(const TreeNode *node);
    TreeNode* find_node(const Key& key);
    const TreeNode* find_node(const Key& key) const;
    
    void replaceParentChild(TreeNode *old_node, TreeNode *new_node);
    
    template<class P>
    std::pair<TreeNode*, bool> insertImpl(P&& val);
    
    void erase_node(TreeNode *node);
    
public:
    template<bool IsConst>
    class iter {
    private:
        using NodePtr = std::conditional_t<IsConst, const TreeNode*, TreeNode*>;
        NodePtr ptr;
        const bst *container;

        friend class bst;

        iter(NodePtr n, const bst *c) : ptr(n), container(c) {}

        NodePtr next(NodePtr n) const {
            if (!n) {
                return nullptr;
            }
            if (n->right) {
                return bst::minNode(n->right);
            }
            NodePtr parent = n->parent;
            while (parent && n == parent->right) {
                n = parent;
                parent = parent->parent;
            }
            return parent;
        }

        NodePtr prev(NodePtr n) const {
            if (!n) {
                return bst::maxNode(container->root);
            }
            if (n->left) {
                return bst::maxNode(n->left);
            }
            NodePtr parent = n->parent;
            while (parent && n == parent->left) {
                n = parent;
                parent = parent->parent;
            }
            return parent;
        }

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::pair<const Key, T>;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<IsConst, const value_type*, value_type*>;
        using reference = std::conditional_t<IsConst, const value_type&, value_type&>;

        iter() : ptr(nullptr), container(nullptr) {}
        template<bool OtherConst, typename = std::enable_if_t<IsConst && !OtherConst>>
        iter(const iter<OtherConst>& other) : ptr(other.ptr), container(other.container) {}

        reference operator*() const {
            return ptr->value;
        }

        pointer operator->() const {
            return &(ptr->value);
        }

        iter& operator++() {
            ptr = next(ptr);
            return *this;
        }

        iter operator++(int) {
            iter temp = *this;
            operator++();
            return temp;
        }

        iter& operator--() {
            ptr = prev(ptr);
            return *this;
        }

        iter operator--(int) {
            iter temp = *this;
            operator--();
            return temp;
        }

        bool operator==(const iter& other) const {
            return ptr == other.ptr && container == other.container;
        }

        bool operator!=(const iter& other) const {
            return !(*this == other);
        }
    };

    using iterator = iter<false>;
    using const_iterator = iter<true>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    bst();
    explicit bst(const Compare& comp, const Allocator& alloc = Allocator());
    explicit bst(const Allocator& alloc);
    bst(const bst& other);
    bst(bst&& other) noexcept;
    bst& operator=(const bst& other);
    bst& operator=(bst&& other) noexcept;
    ~bst();

    T& operator[](const Key& key);
    T& at(const Key& key);
    const T& at(const Key& key) const;
    std::pair<iterator, bool> insert(const std::pair<const Key, T>& val);
    std::pair<iterator, bool> insert(std::pair<const Key, T>&& val);
    size_t erase(const Key& key);
    iterator erase(iterator pos);
    iterator find(const Key& key);
    const_iterator find(const Key& key) const;
    bool contains(const Key& key) const;
    bool empty() const;
    size_t size() const;
    void clear();
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
};

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::TreeNode*
bst<Key, T, Compare, Allocator>::createNode(const std::pair<const Key, T>& val, TreeNode *parent) {
    TreeNode *n = NodeTraits::allocate(alloc, 1);
    try {
        NodeTraits::construct(alloc, n, val, parent);
    } catch (...) {
        NodeTraits::deallocate(alloc, n, 1);
        throw;
    }
    return n;
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::TreeNode*
bst<Key, T, Compare, Allocator>::createNode(std::pair<const Key, T>&& val, TreeNode *parent) {
    TreeNode *n = NodeTraits::allocate(alloc, 1);
    try {
        NodeTraits::construct(alloc, n, std::move(val), parent);
    } catch (...) {
        NodeTraits::deallocate(alloc, n, 1);
        throw;
    }
    return n;
}

template<class Key, class T, class Compare, class Allocator>
void bst<Key, T, Compare, Allocator>::destroyNode(TreeNode *node) {
    NodeTraits::destroy(alloc, node);
    NodeTraits::deallocate(alloc, node, 1);
}

template<class Key, class T, class Compare, class Allocator>
void bst<Key, T, Compare, Allocator>::clearRec(TreeNode *node) {
    if (!node) {
        return;
    }
    clearRec(node->left);
    clearRec(node->right);
    destroyNode(node);
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::TreeNode*
bst<Key, T, Compare, Allocator>::minNode(TreeNode *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template<class Key, class T, class Compare, class Allocator>
const typename bst<Key, T, Compare, Allocator>::TreeNode*
bst<Key, T, Compare, Allocator>::minNode(const TreeNode *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::TreeNode*
bst<Key, T, Compare, Allocator>::maxNode(TreeNode *node) {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

template<class Key, class T, class Compare, class Allocator>
const typename bst<Key, T, Compare, Allocator>::TreeNode*
bst<Key, T, Compare, Allocator>::maxNode(const TreeNode *node) {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::TreeNode*
bst<Key, T, Compare, Allocator>::find_node(const Key& key) {
    TreeNode *cur = root;
    while (cur) {
        if (comp(key, cur->value.first)) {
            cur = cur->left;
        } else if (comp(cur->value.first, key)) {
            cur = cur->right;
        } else {
            return cur;
        }
    }
    return nullptr;
}

template<class Key, class T, class Compare, class Allocator>
const typename bst<Key, T, Compare, Allocator>::TreeNode*
bst<Key, T, Compare, Allocator>::find_node(const Key& key) const {
    const TreeNode *cur = root;
    while (cur) {
        if (comp(key, cur->value.first)) {
            cur = cur->left;
        } else if (comp(cur->value.first, key)) {
            cur = cur->right;
        } else {
            return cur;
        }
    }
    return nullptr;
}

template<class Key, class T, class Compare, class Allocator>
void bst<Key, T, Compare, Allocator>::replaceParentChild(TreeNode *old_node, TreeNode *new_node) {
    if (old_node == root) {
        root = new_node;
        if (new_node) {
            new_node->parent = nullptr;
        }
    } else {
        if (old_node->parent->left == old_node) {
            old_node->parent->left = new_node;
        } else {
            old_node->parent->right = new_node;
        }
        if (new_node) {
            new_node->parent = old_node->parent;
        }
    }
}

template<class Key, class T, class Compare, class Allocator>
template<class P>
std::pair<typename bst<Key, T, Compare, Allocator>::TreeNode*, bool>
bst<Key, T, Compare, Allocator>::insertImpl(P&& val) {
    const Key& k = val.first;
    
    if (!root) {
        root = createNode(std::forward<P>(val));
        size_++;
        return {root, true};
    }

    TreeNode *p = root;
    TreeNode *parent = nullptr;

    while (p) {
        parent = p;
        if (comp(k, p->value.first)) {
            p = p->left;
        } else if (comp(p->value.first, k)) {
            p = p->right;
        } else {
            return {p, false};
        }
    }

    TreeNode *x = createNode(std::forward<P>(val), parent);
    if (comp(k, parent->value.first)) {
        parent->left = x;
    } else {
        parent->right = x;
    }
    size_++;
    return {x, true};
}

template<class Key, class T, class Compare, class Allocator>
void bst<Key, T, Compare, Allocator>::erase_node(TreeNode *node) {
    if (!node) {
        return;
    }

    if (!node->left && !node->right) {
        replaceParentChild(node, nullptr);
        destroyNode(node);
        size_--;
    } else if (!node->left || !node->right) {
        TreeNode *child = node->left ? node->left : node->right;
        replaceParentChild(node, child);
        destroyNode(node);
        size_--;
    } else {
        TreeNode *succ = bst::minNode(node->right);
        
        if (succ->parent != node) {
            replaceParentChild(succ, succ->right);
            succ->right = node->right;
            if (node->right) {
                node->right->parent = succ;
            }
        }
        
        succ->left = node->left;
        if (node->left) {
            node->left->parent = succ;
        }
        
        replaceParentChild(node, succ);
        destroyNode(node);
        size_--;
    }
}

template<class Key, class T, class Compare, class Allocator>
bst<Key, T, Compare, Allocator>::bst() : root(nullptr), size_(0), comp(), alloc() {}

template<class Key, class T, class Compare, class Allocator>
bst<Key, T, Compare, Allocator>::bst(const Compare& comp, const Allocator& alloc)
    : root(nullptr), size_(0), comp(comp), alloc(alloc) {}

template<class Key, class T, class Compare, class Allocator>
bst<Key, T, Compare, Allocator>::bst(const Allocator& alloc)
    : root(nullptr), size_(0), comp(), alloc(alloc) {}

template<class Key, class T, class Compare, class Allocator>
bst<Key, T, Compare, Allocator>::bst(const bst& other)
    : root(nullptr), size_(0), comp(other.comp), alloc(other.alloc) {
    try {
        for (const auto& pair : other) {
            insert(pair);
        }
    } catch (...) {
        clear();
        throw;
    }
}

template<class Key, class T, class Compare, class Allocator>
bst<Key, T, Compare, Allocator>::bst(bst&& other) noexcept
    : root(other.root), size_(other.size_), comp(std::move(other.comp)),
      alloc(std::move(other.alloc)) {
    other.root = nullptr;
    other.size_ = 0;
}

template<class Key, class T, class Compare, class Allocator>
bst<Key, T, Compare, Allocator>& bst<Key, T, Compare, Allocator>::operator=(const bst& other) {
    if (this != &other) {
        bst temp(other);

        size_ = temp.size_;
        std::swap(root, temp.root);
        std::swap(comp, temp.comp);
        std::swap(alloc, temp.alloc);
    }
    return *this;
}

template<class Key, class T, class Compare, class Allocator>
bst<Key, T, Compare, Allocator>& bst<Key, T, Compare, Allocator>::operator=(bst&& other) noexcept {
    if (this != &other) {
        clear();
        root = other.root;
        size_ = other.size_;
        comp = std::move(other.comp);
        alloc = std::move(other.alloc);
        other.root = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template<class Key, class T, class Compare, class Allocator>
bst<Key, T, Compare, Allocator>::~bst() {
    clear();
}

template<class Key, class T, class Compare, class Allocator>
T& bst<Key, T, Compare, Allocator>::operator[](const Key& key) {
    auto result = insertImpl(std::pair<const Key, T>(key, T()));
    return result.first->value.second;
}

template<class Key, class T, class Compare, class Allocator>
T& bst<Key, T, Compare, Allocator>::at(const Key& key) {
    TreeNode *n = find_node(key);
    if (!n) {
        throw std::out_of_range("key not found");
    }
    return n->value.second;
}

template<class Key, class T, class Compare, class Allocator>
const T& bst<Key, T, Compare, Allocator>::at(const Key& key) const {
    const TreeNode *n = find_node(key);
    if (!n) {
        throw std::out_of_range("key not found");
    }
    return n->value.second;
}

template<class Key, class T, class Compare, class Allocator>
std::pair<typename bst<Key, T, Compare, Allocator>::iterator, bool>
bst<Key, T, Compare, Allocator>::insert(const std::pair<const Key, T>& val) {
    auto result = insertImpl(val);
    return {iterator(result.first, this), result.second};
}

template<class Key, class T, class Compare, class Allocator>
std::pair<typename bst<Key, T, Compare, Allocator>::iterator, bool>
bst<Key, T, Compare, Allocator>::insert(std::pair<const Key, T>&& val) {
    auto result = insertImpl(std::move(val));
    return {iterator(result.first, this), result.second};
}

template<class Key, class T, class Compare, class Allocator>
size_t bst<Key, T, Compare, Allocator>::erase(const Key& key) {
    TreeNode *n = find_node(key);
    if (!n) {
        return 0;
    }
    erase_node(n);
    return 1;
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::iterator
bst<Key, T, Compare, Allocator>::erase(iterator pos) {
    if (pos == end()) {
        return end();
    }

    iterator next = pos;
    next++;
    erase_node(pos.ptr);
    return next;
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::iterator
bst<Key, T, Compare, Allocator>::find(const Key& key) {
    return iterator(find_node(key), this);
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::const_iterator
bst<Key, T, Compare, Allocator>::find(const Key& key) const {
    return const_iterator(find_node(key), this);
}

template<class Key, class T, class Compare, class Allocator>
bool bst<Key, T, Compare, Allocator>::contains(const Key& key) const {
    return find_node(key) != nullptr;
}

template<class Key, class T, class Compare, class Allocator>
bool bst<Key, T, Compare, Allocator>::empty() const {
    return size_ == 0;
}

template<class Key, class T, class Compare, class Allocator>
size_t bst<Key, T, Compare, Allocator>::size() const {
    return size_;
}

template<class Key, class T, class Compare, class Allocator>
void bst<Key, T, Compare, Allocator>::clear() {
    clearRec(root);
    root = nullptr;
    size_ = 0;
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::iterator
bst<Key, T, Compare, Allocator>::begin() {
    return iterator(bst::minNode(root), this);
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::const_iterator
bst<Key, T, Compare, Allocator>::begin() const {
    return const_iterator(bst::minNode(root), this);
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::iterator
bst<Key, T, Compare, Allocator>::end() {
    return iterator(nullptr, this);
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::const_iterator
bst<Key, T, Compare, Allocator>::end() const {
    return const_iterator(nullptr, this);
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::reverse_iterator
bst<Key, T, Compare, Allocator>::rbegin() {
    return reverse_iterator(end());
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::const_reverse_iterator
bst<Key, T, Compare, Allocator>::rbegin() const {
    return const_reverse_iterator(end());
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::reverse_iterator
bst<Key, T, Compare, Allocator>::rend() {
    return reverse_iterator(begin());
}

template<class Key, class T, class Compare, class Allocator>
typename bst<Key, T, Compare, Allocator>::const_reverse_iterator
bst<Key, T, Compare, Allocator>::rend() const {
    return const_reverse_iterator(begin());
}
