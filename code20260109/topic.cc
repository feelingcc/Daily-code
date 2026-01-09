/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
// class Solution {
// public:
//     // 返回值表示找到的最近公共祖先
//     TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
//         if(root == nullptr) return nullptr;
//         // 是当前p或q其中一个节点直接返回当前节点
//         if(root == p || root == q) return root;

//         TreeNode* left = lowestCommonAncestor(root->left , p , q);
//         TreeNode* right = lowestCommonAncestor(root->right , p , q);

//         // 如果当前子树中找到p和q了，那么当前节点就是最近的公共祖先
//         if(left && right) {
//             return root;
//         } else if(left) {
//             // 左子树找到了，但是右子树没有找到，说明公共祖先还得继续向上找
//             return left;
//         } else if(right) {
//             // 类似
//             return right;
//         } else {
//             return nullptr;
//         }
//     }
// };

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
// class Solution {
// public:
//     // 这道题的本质是，寻找最深节点的公共祖先，可以通过一次dfs去记录最深节点的公共祖先
//     // 1. pair 的第一个 int 不是从根向下的深度，而是从当前节点向下的最大高度
//     // 2. TreeNode 保存的是当前最深节点的公共祖先
//     pair<int , TreeNode*> sub(TreeNode* root) {
//         if(root == nullptr) return {0 , nullptr};
//         // 后序遍历
//         auto left = sub(root->left);
//         auto right = sub(root->right);
//         // left.first 是当前所在左子树的最大高度
//         // right.first 是当前所在右子树的最大高度
//         if(left.first > right.first) {
//             // 最深节点都在左子树里，那么这些节点的最近公共祖先（LCA）肯定也在左子树中，所以直接沿用左子树传上来的 LCA 结果就行。
//             return {left.first + 1 , left.second};
//         } else if(right.first > left.first) {
//             return {right.first + 1 , right.second};
//         } else {
//             return {left.first + 1 , root};
//         }
//     }
//     TreeNode* subtreeWithAllDeepest(TreeNode* root) {
//         return sub(root).second;
//     }
// };