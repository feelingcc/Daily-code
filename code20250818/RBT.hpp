#pragma once
#include <iostream>
#include <utility>
#include <cassert>

enum Color
{
    RED,
    BLACK
};

namespace RBT
{
    template <typename Key, typename Value>
    struct RBTreeNode
    {
        std::pair<Key, Value> data;
        RBTreeNode<Key, Value> *left;
        RBTreeNode<Key, Value> *right;
        RBTreeNode<Key, Value> *parent;
        Color color;
        RBTreeNode(const std::pair<Key, Value> &_data)
            : data(_data), left(nullptr), right(nullptr), parent(nullptr), color(RED) // 默认插入节点是红色
        {
        }
    };

    template <typename Key, typename Value>
    class RBTree
    {
        using TreeNode = RBTreeNode<Key, Value>;

    public:
        bool insert(const std::pair<Key, Value> &data)
        {
            if (root == nullptr)
            {
                root = new TreeNode(data);
                root->color = BLACK; // 根节点必须是黑色
                return true;
            }

            TreeNode *parent = nullptr;
            TreeNode *cur = root;
            while (cur)
            {
                if (cur->data.first < data.first)
                {
                    parent = cur;
                    cur = cur->right;
                }
                else if (cur->data.first > data.first)
                {
                    parent = cur;
                    cur = cur->left;
                }
                else
                {
                    return false; // 存在相同元素不插入
                }
            }
            // 新插入节点为红色
            cur = new TreeNode(data);
            if (parent->data.first < cur->data.first)
                parent->right = cur;
            else
                parent->left = cur;
            cur->parent = parent;

            // 父亲是红色，出现连续的红色节点，需要处理
            while (parent && parent->color == RED)
            {
                TreeNode *g = parent->parent;
                if (g->left == parent)
                {
                    TreeNode *u = g->right;
                    if (u && u->color == RED)
                    {
                        // 情况1：变色处理 + 继续向上处理
                        parent->color = u->color = BLACK;
                        g->color = RED;
                        cur = g;
                        parent = cur->parent;
                    }
                    else
                    {
                        // u == nullptr || u->color == BLACK
                        if (parent->left == cur)
                        {
                            // 情况2：右单旋 + 变色
                            rotateR(g);
                            g->color = RED;
                            parent->color = BLACK;
                        }
                        else
                        {
                            // 情况3：左右双旋 + 变色
                            rotateL(parent);
                            rotateR(g);
                            cur->color = BLACK;
                            g->color = RED;
                        }
                        break; // 无需继续向上更新
                    }
                }
                else
                {
                    TreeNode *u = g->left;
                    if (u && u->color == RED)
                    {
                        // 情况1：变色处理 + 继续向上处理
                        parent->color = u->color = BLACK;
                        g->color = RED;
                        cur = g;
                        parent = cur->parent;
                    }
                    else
                    {
                        // u == nullptr || u->color == BLACK
                        if (parent->right == cur)
                        {
                            // 情况2：左单旋 + 变色
                            rotateL(g);
                            g->color = RED;
                            parent->color = BLACK;
                        }
                        else
                        {
                            // 情况3：右左双旋 + 变色
                            rotateR(parent);
                            rotateL(g);
                            cur->color = BLACK;
                            g->color = RED;
                        }
                        break; // 无需继续向上更新
                    }
                }
            }
            root->color = BLACK; // root始终为黑色
            return true;
        }

        // 旋转里会改变root
        void rotateR(TreeNode *parent)
        {
            TreeNode *subL = parent->left;
            TreeNode *subLR = subL->right;
            TreeNode *pParent = parent->parent;

            parent->left = subLR;
            // 当h = 0时，subLR可能为空
            if (subLR)
            {
                subLR->parent = parent;
            }
            subL->right = parent;
            parent->parent = subL;
            if (root == parent)
            {
                // 更新到根发生不平衡
                root = subL;
                subL->parent = nullptr;
            }
            else
            {
                // 只是局部树发生不平衡
                if (pParent->left == parent)
                    pParent->left = subL;
                else
                    pParent->right = subL;
                subL->parent = pParent;
            }
        }

        void rotateL(TreeNode *parent)
        {
            TreeNode *subR = parent->right;
            TreeNode *subRL = subR->left;
            TreeNode *pParent = parent->parent;

            parent->right = subRL;
            // 当h = 0时，subRL可能为空
            if (subRL)
            {
                subRL->parent = parent;
            }
            subR->left = parent;
            parent->parent = subR;
            if (root == parent)
            {
                // 更新到根发生不平衡
                root = subR;
                subR->parent = nullptr;
            }
            else
            {
                // 只是局部树发生不平衡
                if (pParent->left == parent)
                    pParent->left = subR;
                else
                    pParent->right = subR;
                subR->parent = pParent;
            }
        }

    private:
        TreeNode *root = nullptr;

        // test code
    public:
        void inorder()
        {
            _inorder(root);
            std::cout << std::endl;
        }

        bool isRBT()
        {
            if (root == nullptr)
                return true;

            if (root->color == RED)
                return false;

            // 参考值
            int refNum = 0;
            TreeNode*cur = root;
            while (cur)
            {
                if (cur->color == BLACK)
                {
                    ++refNum;
                }
                cur = cur->left;
            }

            return Check(root, 0, refNum);
        }

    private:
        void _inorder(TreeNode *root)
        {
            if (root == nullptr)
            {
                return;
            }

            _inorder(root->left);
            std::cout << root->data.first << ":" << root->data.second << std::endl;
            _inorder(root->right);
        }

        bool Check(TreeNode *root, int blackNum, const int refNum)
        {
            if (root == nullptr)
            {
                // 前序遍历走到空时，意味着一条路径走完了
                // cout << blackNum << endl;
                if (refNum != blackNum)
                {
                    std::cout << "存在黑色结点的数量不相等的路径" << std::endl;
                    return false;
                }
                return true;
            }

            // 检查孩子不太方便，因为孩子有两个，且不一定存在，反过来检查父亲就方便多了
            if (root->color == RED && root->parent->color == RED)
            {
                std::cout << root->data.first << "存在连续的红色结点" << std::endl;
                return false;
            }

            if (root->color == BLACK)
            {
                blackNum++;
            }

            return Check(root->left, blackNum, refNum) && Check(root->right, blackNum, refNum);
        }
    };
} // namespace RBT end