#pragma once
#include <iostream>
#include <utility>
#include <cassert>

namespace AVL {
    template<typename Key , typename Value>
    struct AVLTreeNode {
        std::pair<Key , Value> data;
        AVLTreeNode<Key , Value>* left;
        AVLTreeNode<Key , Value>* right;
        AVLTreeNode<Key , Value>* parent;
        int bf; // balance factor
        AVLTreeNode(const std::pair<Key , Value>& _data)
            :data(_data) , left(nullptr) , right(nullptr) , parent(nullptr) , bf(0)
        {}
    };

    template<typename Key , typename Value>
    class AVLTree {
        using TreeNode = AVLTreeNode<const Key , Value>;
        public:
            bool insert(const std::pair<Key , Value>& data) {
                if (root == nullptr) {
                    root = new TreeNode(data);
                    return true;
                }

                TreeNode* parent = nullptr;
                TreeNode* cur = root;
                while (cur) {
                    if (cur->data.first < data.first) {
                        parent = cur;
                        cur = cur->right;
                    } else if (cur->data.first > data.first) {
                        parent = cur;
                        cur = cur->left;
                    } else {
                        return false;   // 存在相同元素不插入
                    }
                }
                cur = new TreeNode(data);
                if (parent->data.first < cur->data.first)
                    parent->right = cur;
                else 
                    parent->left = cur;
                cur->parent = parent;
                
                // 更新cur祖先路径上节点的平衡因子
                while (parent) {
                    // 判断在parent左还是右插入节点，parent平衡因子++还是--
                    if (parent->left == cur) {
                        parent->bf--;
                    } else {
                        parent->bf++;
                    }
                    
                    if (parent->bf == 0) {
                        // 1.当前节点的平衡因子变为 0，无需向上更新
                        break;
                    } else if (parent->bf == 1 || parent->bf == -1) {
                        // 2.当前节点的平衡因子变为1或-1，继续向上更新
                        cur = parent;
                        parent = parent->parent;
                    } else if (parent->bf == 2 || parent->bf == -2) {
                        // 3.当前节点的平衡因子变为2或-2，旋转处理，旋转后无需向上更新
                        if (cur->bf == -1 && parent->bf == -2) {
                            // 右单旋
                        } else if (cur->bf == 1 && parent->bf == 2) {
                            // 左单旋
                        } else if () {
                            // 右左双旋
                        } else if () {
                            // 左右双旋
                        } else {
                            assert(false);
                        }
                        break;
                    } else {
                        assert(false);
                    }
                }

                return true;
            }

            void rotateR(TreeNode* parent) {
                TreeNode* subL = parent->left;
                TreeNode* subLR = subL->right;
                TreeNode* pParent = parent->parent;

                parent->left = subLR;
                // 当h = 0时，subLR可能为空
                if (subLR) {
                    subLR->parent = parent;
                }
                subL->right = parent;
                parent->parent = subL;
                if (root == parent) {
                    // 更新到根发生不平衡
                    root = subL;
                    subL->parent = nullptr;
                } else {
                    // 只是局部树发生不平衡
                    if (pParent->left == parent)
                        pParent->left = subL;
                    else
                        pParent->right = subL;
                    subL->parent = pParent;
                }

                // 更新平衡因子
                subL->bf = parent->bf = 0;
            }

            void rotateL(TreeNode* parent) {
                TreeNode* subR = parent->right;
                TreeNode* subRL = subR->left;
                TreeNode* pParent = parent->parent;
                
                parent->right = subRL;
                // 当h = 0时，subRL可能为空
                if (subRL) {
                    subRL->parent = parent;
                }
                subR->left = parent;
                parent->parent = subR;
                if (root == parent) {
                    // 更新到根发生不平衡
                    root = subR;
                    subR->parent = nullptr;
                } else {
                    // 只是局部树发生不平衡
                    if (pParent->left == parent)
                        pParent->left = subR;
                    else
                        pParent->right = subR;
                    subR->parent = pParent;
                }

                // 更新平衡因子
                subR->bf = parent->bf = 0;
            }
        private:
            TreeNode root = nullptr;
    };
}