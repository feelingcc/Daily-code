#pragma once
#include <iostream>

namespace binary_search_tree {
    template<typename Key>
    struct TreeNode {
        Key key;
        TreeNode<Key>* left;
        TreeNode<Key>* right;

        TreeNode(const Key& val = Key())
            :key(val) , left(nullptr) , right(nullptr)
        {}
    };

    template<typename Key>
    class BinarySearchTree {
        using TNode = TreeNode<Key>;

        void _inorder(TNode* root) {
            if (root == nullptr) return;
            _inorder(root->left);
            std::cout << root->key << " ";
            _inorder(root->right);
        }

        TNode* _build(TNode* root) {
            if (root == nullptr) return nullptr;
            TNode* newNode = new TNode(root->key);
            newNode->left = _build(root->left);
            newNode->right = _build(root->right);
        }

        void _clear(TNode* root) {
            if (root == nullptr) return;
            _clear(root->left);
            _clear(root->right);
            delete root;
        }

        public:
            BinarySearchTree() = default;

            BinarySearchTree(const BinarySearchTree& bst) {
                root = _build(bst->root);
            }

            BinarySearchTree& operator=(BinarySearchTree temp) {
                std::swap(root , temp->root);
                return *this;
            }

            ~BinarySearchTree() {
                _clear(root);
                root = nullptr;
            }

            void inorder() const {
                _inorder(root);
                std::cout << std::endl;
            }

            bool insert(const Key& key);
            bool erase(const Key& key);
            bool find(const Key& key);
        private:
            TNode* root;
    };

    template<typename Key>
    bool BinarySearchTree<Key>::insert(const Key& key) {
        if (root == nullptr) {
            root = new TNode(key);
            return true;
        }

        TNode* prev = nullptr;
        TNode* cur = root;
        while (cur) {
            if (key > cur->key) {
                prev = cur;
                cur = cur->right;
            }
            else if (key < cur->key) {
                prev = cur;
                cur = cur->left;
            }
            else {
                return false;  // 重复不插入
            }
        }
        cur = new TNode(key);
        if (cur->key > prev->key) prev->right = cur;
        else prev->left = cur;

        return true;
    }

    template<typename Key>
    bool BinarySearchTree<Key>::erase(const Key& key) {
        if (root == nullptr)
            return false;
        
        TNode* prev = nullptr;
        TNode* cur = root;
        while (cur) {
            if (key > cur->key) {
                prev = cur;
                cur = cur->right;
            }
            else if (key < cur->key) {
                prev = cur;
                cur = cur->left;
            }
            else {
                // 1.删除的节点是叶子节点
                // 2.删除的节点是只有一个孩子的节点(情况1可以被情况2包含)
                // 3.删除的节点是有两个孩子的节点
                if (cur->left == nullptr) {
                    if (prev == nullptr) {
                        root = cur->right;
                    } else {
                        if (prev->left == cur) {
                            prev->left = cur->right;
                        } else {
                            prev->right = cur->right;
                        }
                    }
                    delete cur;
                } else if (cur->right == nullptr) {
                    if (prev == nullptr) {
                        root = cur->left;
                    } else {
                        if (prev->left == cur) {
                            prev->left = cur->left;
                        } else {
                            prev->right = cur->left;
                        }
                    }
                    delete cur;
                } else {
                    // 替换法删除有两个孩子的节点
                    // 寻找右子树的最左节点
                    TNode* replaceParentNode = cur;
                    TNode* replaceNode = cur->right;
                    while (replaceNode->left) {
                        replaceParentNode = replaceNode;
                        replaceNode = replaceNode->left;
                    }
                    std::swap(cur->key , replaceNode->key);
                    if (replaceParentNode->left = replaceNode) {
                        // 已经是最左节点了，所以只需要考虑右孩子
                        replaceParent->left = replace->_right;
                        // if (replaceNode->left == nullptr)
                        //     replaceParentNode->left = replaceNode->right;
                        // else 
                        //     replaceParentNode->left = replaceNode->left;
                    } else {
                        // 已经是最左节点了，所以只需要考虑右孩子
                        replaceParent->right = replace->_right;
                        // if (replaceNode->left == nullptr)
                        //     replaceParentNode->right = replaceNode->right;
                        // else 
                        //     replaceParentNode->right = replaceNode->left;
                    }
                    delete replaceNode;
                }
                return true;
            }
        }
        return false;
    }

    template<typename Key>
    bool BinarySearchTree<Key>::find(const Key& key) {
        TNode* cur = root;
        while (cur) {
            if (key > cur->key) cur = cur->right;
            else if (key < cur->key) cur = cur->left;
            else return true; 
        }
        return false;
    }
}