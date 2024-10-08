#pragma once

#include <functional>
#include <vector>
#include <stack>

template<typename T>
class BinaryTree {
private:
    static inline const auto m_end = nullptr;
    using Type = T;

    struct TreeNode {
        friend class BinaryTree;

    private:
        Type m_val;
        TreeNode *left;
        TreeNode *right;

    public:
        TreeNode(Type val, TreeNode *left, TreeNode *right);
        TreeNode();
        TreeNode(Type val);
    };

    TreeNode *m_root;
    static inline size_t m_size = 0;

    void deleter() noexcept;

public:

    BinaryTree(TreeNode *root);
    BinaryTree();
    BinaryTree(BinaryTree &other) = delete;
    BinaryTree(BinaryTree &&other) noexcept;

    ~BinaryTree();

    BinaryTree &operator=(const BinaryTree &other) = delete;
    BinaryTree &operator=(BinaryTree &&other) noexcept;

    bool is_balanced() const noexcept;
    bool is_empty() const noexcept;
    bool find(const Type val) const noexcept;

    size_t get_size() const noexcept;

    int height() const noexcept;

    void inserter(const Type val) noexcept;
    void balancer();
    void delete_val(const Type val);

    TreeNode *get_pointer(const Type val) const noexcept;
    TreeNode *get_pointer_min() const noexcept;
    TreeNode *get_pointer_max() const noexcept;

    std::vector<T> in_order_traversal() const noexcept;
    std::vector<T> pre_order_traversal() const noexcept;
    std::vector<T> post_order_traversal() const noexcept;
};

template<typename T>
BinaryTree<T>::TreeNode::TreeNode(T val, TreeNode *left, TreeNode *right) : m_val(val), left(left), right(right) {}

template<typename T>
BinaryTree<T>::TreeNode::TreeNode() : TreeNode(0, m_end, m_end) {}

template<typename T>
BinaryTree<T>::TreeNode::TreeNode(T val) : TreeNode(val, m_end, m_end) {}

template<typename T>
BinaryTree<T>::BinaryTree(TreeNode *root) : m_root(root) {}

template<typename T>
BinaryTree<T>::BinaryTree() : BinaryTree(m_end) {}

template<typename T>
BinaryTree<T>::BinaryTree(BinaryTree &&other) noexcept
{
    m_root = other.m_root;
    m_size = other.m_size;
    other.m_root = m_end;
    other.m_size = 0;
}

template<typename T>
BinaryTree<T>::~BinaryTree() {
    deleter();
}

template<typename T>
BinaryTree<T> &BinaryTree<T>::operator=(BinaryTree &&other) noexcept {
if (this == &other)
return *this;
delete m_root;
m_root = other.m_root;
m_size = other.m_size;
other.m_root = m_end;
other.m_size = 0;
return *this;
}

template<typename T>
void BinaryTree<T>::deleter() noexcept{
    TreeNode *curr = m_root;
    std::stack<TreeNode *> st;
    TreeNode *last_visited = m_end;

    while (curr || !st.empty()) {
        while (curr) {
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top();
        if (!curr->right || last_visited == curr->right) {
            st.pop();
            last_visited = curr;
            delete curr;
            curr = m_end;
        } else
            curr = curr->right;
    }
}

template<typename T>
bool BinaryTree<T>::is_balanced() const noexcept{
    bool flag = true;
    TreeNode* curr = m_root;

    std::function<int(TreeNode*, bool)> fu = [&](TreeNode* curr, bool flag) {
        if (!curr)
            return 0;
        int leftHeight = fu(curr->left, flag);
        int rightHeight = fu(curr->right, flag);

        if (abs(leftHeight - rightHeight) > 1)
            flag = false;

        return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    };

    fu(curr, flag);
    return flag;
}

template<typename T>
bool BinaryTree<T>::is_empty() const noexcept {
return !m_size;
}

template<typename T>
bool BinaryTree<T>::find(const T val) const noexcept{
    if (!m_root)
        return false;

    auto *curr = m_root;

    while (curr) {
        if (curr->m_val == val)
            return true;
        if (curr->m_val > val)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return false;
}

template<typename T>
size_t BinaryTree<T>::get_size() const noexcept {
return m_size;
}

template<typename T>
int BinaryTree<T>::height() const noexcept{
    auto count = 0;
    if (!m_root)
        return count;
    std::stack<TreeNode *> m_stack;
    m_stack.push(m_root);
    TreeNode *root = m_end;
    while (!m_stack.empty()) {
        count++;
        auto stack_size = m_stack.size();
        for (auto i = 0; i < stack_size; i++) {
            root = m_stack.top();
            m_stack.pop();
            if (root->left)
                m_stack.push(root->left);
            if (root->right)
                m_stack.push(root->right);
        }
    }
    return count;
}

template<typename T>
void BinaryTree<T>::inserter(const T val) noexcept{
    auto* insert_node = new TreeNode{val};

    if (!m_root) {
        m_root = insert_node;
        m_size++;
        return;
    }

    TreeNode *curr = m_root;
    TreeNode *prev = m_end;

    while (curr) {
        if (curr->m_val == val)
            return;
        prev = curr;
        if (curr->m_val < val)
            curr = curr->right;
        else
            curr = curr->left;
    }

    if (prev->m_val < val)
        prev->right = insert_node;
    else
        prev->left = insert_node;
    m_size++;
}

template<typename T>
void BinaryTree<T>::balancer() {
    std::function<TreeNode *(TreeNode *)> rotate_left = [](TreeNode *curr_root) {
        TreeNode *new_root = curr_root->right;
        new_root->left = curr_root;
        new_root->left->right = curr_root->right->left;
        new_root->right = curr_root->right->right;
        return new_root;
    };
    std::function<TreeNode *(TreeNode *)> rotate_right = [](TreeNode *curr_root) {
        TreeNode *new_root = curr_root->left;
        new_root->right = curr_root;
        new_root->right->left = curr_root->left->right;
        new_root->left = curr_root->left->left;
        return new_root;
    };
}

template<typename T>
void BinaryTree<T>::delete_val(const T val) {
    if (!find(val))
        return;
    if (m_root->m_val == val)
        m_root = m_end;

    TreeNode *found = m_root;
    TreeNode *prev = m_end;
    std::stack<TreeNode *> m_stack;
    bool last_right = 0;

    while (found) {
        m_stack.push(found);
        if (found->m_val == val)
            break;
        if (found->m_val < val) {
            found = found->right;
            last_right = 1;
        } else {
            found = found->left;
            last_right = 0;
        }
    }

    prev = m_stack.top();
    if (!found->right && !found->left) {
        if (last_right)
            prev->right = m_end;
        else
            prev->left = m_end;
    } else if (!found->right && found->left) {
        TreeNode *next = found->left;

        if (last_right)
            prev->right = next;
        else
            prev->left = next;
    } else if (found->right && !found->right->left) {
        TreeNode *next = found->right;

        if (last_right)
            prev->right = next;
        else
            prev->left = next;
    } else {
        TreeNode *substitute = found;
        TreeNode *prev_substitute = m_end;
        std::stack<TreeNode *> substitute_stack;

        while (substitute) {
            substitute_stack.push(substitute);
            substitute = substitute->left;
        }
        substitute = substitute_stack.top();
        substitute_stack.pop();
        prev_substitute = substitute_stack.top();
        prev_substitute->left = m_end;

        auto new_node = new TreeNode{substitute->m_val, found->left, found->right};

        if (last_right)
            prev->right = new_node;
        else
            prev->left = new_node;
        delete substitute;
        substitute = m_end;
    }
    delete found;
    found = m_end;
    m_size--;
}

template<typename T>
typename BinaryTree<T>::TreeNode *BinaryTree<T>::get_pointer(const T val) const noexcept{
    TreeNode *curr = m_root;

    if (!curr)
        return m_end;
    while (curr) {
        if (curr->m_val == val)
            return curr;
        if (curr->m_val > val)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return m_end;
}

template<typename T>
typename BinaryTree<T>::TreeNode *BinaryTree<T>::get_pointer_min() const noexcept{
    TreeNode *curr = m_root;

    while (curr && curr->left)
        curr = curr->left;
    return curr;
}

template<typename T>
typename BinaryTree<T>::TreeNode *BinaryTree<T>::get_pointer_max() const noexcept{
    TreeNode *curr = m_root;

    while (curr && curr->right)
        curr = curr->right;
    return curr;
}

template<typename T>
std::vector<T> BinaryTree<T>::in_order_traversal() const noexcept{
    std::vector<T> result;
    TreeNode *curr = m_root;
    std::stack<TreeNode *> m_stack;

    while (curr || !m_stack.empty()) {
        while (curr) {
            m_stack.push(curr);
            curr = curr->left;
        }
        curr = m_stack.top();
        m_stack.pop();
        result.push_back(curr->m_val);
        curr = curr->right;
    }
    return result;
}

template<typename T>
std::vector<T> BinaryTree<T>::pre_order_traversal() const noexcept{
    std::vector<T> result;
    TreeNode *curr = m_root;
    std::stack<TreeNode *> m_stack;

    while (curr || !m_stack.empty()) {
        while (curr) {
            m_stack.push(curr);
            result.push_back(curr->m_val);
            curr = curr->left;
        }
        curr = m_stack.top();
        m_stack.pop();
        curr = curr->right;
    }
    return result;
}

template<typename T>
std::vector<T> BinaryTree<T>::post_order_traversal() const noexcept{
    std::vector<T> result;
    TreeNode *curr = m_root;
    std::stack<TreeNode *> m_stack;
    TreeNode *last_visited = m_end;

    while (curr || !m_stack.empty()) {
        while (curr) {
            m_stack.push(curr);
            curr = curr->left;
        }
        curr = m_stack.top();
        if (!curr->right || last_visited == curr->right) {
            result.push_back(curr->m_val);
            m_stack.pop();
            last_visited = curr;
            curr = m_end;
        } else
            curr = curr->right;
    }
    return result;
}