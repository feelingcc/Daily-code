#pragma once
#include <iostream>
#include <utility>
#include <cassert>

enum Color{
    RED , BLACK
};

namespace RBT {
    template <typename Key, typename Value>
    struct RBTreeNode
    {
        std::pair<Key, Value> data;
        RBTreeNode<Key, Value> *left;
        RBTreeNode<Key, Value> *right;
        RBTreeNode<Key, Value> *parent;
        Color color;
        RBTreeNode(const std::pair<Key, Value> &_data)
            : data(_data), left(nullptr), right(nullptr), parent(nullptr),color(RED)    // 默认插入节点是红色
        {
        }
    };

    template<typename Key , typename Value>
    class RBTree {
        using TreeNode = RBTreeNode<Key , Value>;
        public:
            bool insert(const std::pair<Key, Value> &data)
        {
            if (root == nullptr)
            {
                root = new TreeNode(data);  
                root->color = BLACK;    // 根节点必须是黑色
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
            cur->color = RED;   
            if (parent->data.first < cur->data.first)
                parent->right = cur;
            else
                parent->left = cur;
            cur->parent = parent;

            // 

        }
        private:
            TreeNode* root = nullptr;
    };
}