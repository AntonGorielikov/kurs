#ifndef TREENODE_H
#define TREENODE_H

#include <QString>

class TreeNode
{
public:
    TreeNode(TreeNode *parent = NULL, QString value = "");
    ~TreeNode();

    TreeNode *get_son() const;
    void set_son(TreeNode *node);
    TreeNode *get_next() const;
    void set_next(TreeNode *node);
    TreeNode *get_parent() const;
    void set_parent(TreeNode *node);
    QString get_value() const;
    void set_value(const QString &value);
    void clear_sons();
    void add_son(TreeNode *node);
    void remove_son(TreeNode *node);
    void remove_son(int index);

private:
    TreeNode *son_;
    TreeNode *next_;
    TreeNode *parent_;
    QString value_;
};

#endif // TREENODE_H
