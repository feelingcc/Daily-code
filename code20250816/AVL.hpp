#pragma once
#include <iostream>
#include <utility>
#include <cassert>

namespace AVL
{
    template <typename Key, typename Value>
    struct AVLTreeNode
    {
        std::pair<Key, Value> data;
        AVLTreeNode<Key, Value> *left;
        AVLTreeNode<Key, Value> *right;
        AVLTreeNode<Key, Value> *parent;
        int bf; // balance factor
        AVLTreeNode(const std::pair<Key, Value> &_data)
            : data(_data), left(nullptr), right(nullptr), parent(nullptr), bf(0)
        {
        }
    };

    template <typename Key, typename Value>
    class AVLTree
    {
        using TreeNode = AVLTreeNode<const Key, Value>;

    public:
        bool insert(const std::pair<Key, Value> &data)
        {
            if (root == nullptr)
            {
                root = new TreeNode(data);
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
            cur = new TreeNode(data);
            if (parent->data.first < cur->data.first)
                parent->right = cur;
            else
                parent->left = cur;
            cur->parent = parent;

            // 更新cur祖先路径上节点的平衡因子
            while (parent)
            {
                // 判断在parent左还是右插入节点，parent平衡因子++还是--
                if (parent->left == cur)
                {
                    parent->bf--;
                }
                else
                {
                    parent->bf++;
                }

                if (parent->bf == 0)
                {
                    // 1.当前节点的平衡因子变为 0，无需向上更新
                    break;
                }
                else if (parent->bf == 1 || parent->bf == -1)
                {
                    // 2.当前节点的平衡因子变为1或-1，继续向上更新
                    cur = parent;
                    parent = parent->parent;
                }
                else if (parent->bf == 2 || parent->bf == -2)
                {
                    // 3.当前节点的平衡因子变为2或-2，旋转处理，旋转后无需向上更新
                    if (cur->bf == -1 && parent->bf == -2)
                    {
                        // 右单旋
                        rotateR(parent);
                    }
                    else if (cur->bf == 1 && parent->bf == 2)
                    {
                        // 左单旋
                        rotateL(parent);
                    }
                    else if (cur->bf == -1 && parent->bf == 2)
                    {
                        // 右左双旋
                        rotateRL(parent);
                    }
                    else if (cur->bf == 1 && parent->bf == -2)
                    {
                        // 左右双旋
                        rotateLR(parent);
                    }
                    else
                    {
                        assert(false);
                    }
                    break;
                }
                else
                {
                    assert(false);
                }
            }

            return true;
        }

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

            // 更新平衡因子
            subL->bf = parent->bf = 0;
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

            // 更新平衡因子
            subR->bf = parent->bf = 0;
        }

        void rotateRL(TreeNode *parent)
        {
            TreeNode *subR = parent->right;
            TreeNode *subRL = subR->left;
            // 提前保存subRL的平衡因子
            int subRLbf = subRL->bf;
            rotateR(subR);
            rotateL(parent);
            // 更新平衡因子
            if (subRLbf == -1)
            {
                // 1.插入节点在subRL的左子树
                parent->bf = 0;
                subR->bf = 1;
                subRL->bf = 0;
            }
            else if (subRLbf == 1)
            {
                // 2.插入节点在subRL的右子树
                parent->bf = -1;
                subR->bf = 0;
                subRL->bf = 0;
            }
            else if (subRLbf == 0)
            {
                // 3.h = 0且subRL为新插入的节点
                parent->bf = 0;
                subR->bf = 0;
                subRL->bf = 0;
            }
            else
            {
                // 不符合AVL树规则
                assert(false);
            }
        }

        void rotateLR(TreeNode *parent)
        {
            TreeNode *subL = parent->left;
            TreeNode *subLR = subL->right;
            // 提前保存subLR的平衡因子
            int subLRbf = subLR->bf;
            rotateL(subL);
            rotateR(parent);
            // 更新平衡因子
            if (subLRbf == -1)
            {
                // 1.插入节点在subRL的左子树
                parent->bf = 1;
                subL->bf = 0;
                subLR->bf = 0;
            }
            else if (subLRbf == 1)
            {
                // 2.插入节点在subRL的右子树
                parent->bf = 0;
                subL->bf = -1;
                subLR->bf = 0;
            }
            else if (subLRbf == 0)
            {
                // 3.h = 0且subRL为新插入的节点
                parent->bf = 0;
                subL->bf = 0;
                subLR->bf = 0;
            }
            else
            {
                // 不符合AVL树规则
                assert(false);
            }
        }
        private:
        TreeNode* root = nullptr;

        public:
        // 以下为 test code
        void InOrder()
        {
            _InOrder(root);
            std::cout << std::endl;
        }

        int Height()
        {
            return _Height(root);
        }

        int Size()
        {
            return _Size(root);
        }

        bool IsBalanceTree()
        {
            return _IsBalanceTree(root);
        }

        TreeNode *Find(const Key &key)
        {
            TreeNode *cur = root;
            while (cur)
            {
                if (cur->data.first < key)
                {
                    cur = cur->right;
                }
                else if (cur->data.first > key)
                {
                    cur = cur->left;
                }
                else
                {
                    return cur;
                }
            }

            return nullptr;
        }

    private:
        void _InOrder(TreeNode *root)
        {
            if (root == nullptr)
            {
                return;
            }

            _InOrder(root->left);
            std::cout << root->data.first << ":" << root->data.second << std::endl;
            _InOrder(root->right);
        }

        int _Height(TreeNode *root)
        {
            if (root == nullptr)
                return 0;
            int leftHeight = _Height(root->left);
            int rightHeight = _Height(root->right);
            return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
        }

        int _Size(TreeNode *root)
        {
            if (root == nullptr)
                return 0;

            return _Size(root->left) + _Size(root->right) + 1;
        }

        bool _IsBalanceTree(TreeNode *root)
        {
            // 空树也是AVL树
            if (nullptr == root)
                return true;
            // 计算pRoot结点的平衡因子：即pRoot左右子树的高度差
            int leftHeight = _Height(root->left);
            int rightHeight = _Height(root->right);
            int diff = rightHeight - leftHeight;

            // 如果计算出的平衡因子与pRoot的平衡因子不相等，或者
            // pRoot平衡因子的绝对值超过1，则一定不是AVL树
            if (abs(diff) >= 2)
            {
                std::cout << root->data.first << "高度差异常" << std::endl;
                return false;
            }

            if (root->bf != diff)
            {
                std::cout << root->data.first << "平衡因子异常" << std::endl;
                return false;
            }

            // pRoot的左和右如果都是AVL树，则该树一定是AVL树
            return _IsBalanceTree(root->left) && _IsBalanceTree(root->right);
        }
    };
}