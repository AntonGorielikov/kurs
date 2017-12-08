#include "treenode.h"

TreeNode::TreeNode(TreeNode *parent, QString value) :
    parent_(parent),
    son_(NULL),
    next_(NULL),
    value_(value)
{
    if (parent)
        parent->add_son(this);
}

TreeNode::~TreeNode()
{
    clear_sons();
}

TreeNode *TreeNode::get_son() const
{
    return son_;
}

void TreeNode::set_son(TreeNode *node)
{
    son_ = node;
}

TreeNode *TreeNode::get_next() const
{
    return next_;
}

void TreeNode::set_next(TreeNode *node)
{
    next_ = node;
}

TreeNode *TreeNode::get_parent() const
{
    return parent_;
}

void TreeNode::set_parent(TreeNode *node)
{
    parent_ = node;
}

TreeNode *TreeNode::get_nson(int num) const
{
    TreeNode *son = son_;
    for (int i = 0; son && i < num; son = son->get_next(), i++);
    return son;
}

int TreeNode::get_nsons() const
{
    int nsons = 0;
    for (TreeNode *son = son_; son; son = son->get_next(), nsons++);
    return nsons;
}

QString TreeNode::get_value() const
{
    return value_;
}

int TreeNode::get_index() const
{
    if (!parent_)
        return -1;

    int index = 0;
    TreeNode *parent_son = parent_->get_son();
    for (; parent_son != this; parent_son = parent_son->get_next());

    return index;
}

void TreeNode::set_value(const QString &value)
{
    value_ = value;
}

void TreeNode::clear_sons()
{
    if (!son_)
        return;

    TreeNode *prev_son = son_,
        *curr_son = prev_son->get_next();

    for (; prev_son->get_next(); prev_son = curr_son, curr_son = curr_son->get_next())
    {
        delete prev_son;
    }

    delete prev_son;
    son_ = NULL;
}

void TreeNode::add_son(TreeNode *node)
{
    TreeNode *son = son_;

    if (!son)
    {
        son_ = node;
        return;
    }

    for (; son->get_next(); son = son->get_next());

    son->set_next(node);
}

void TreeNode::remove_son(TreeNode *node)
{
    TreeNode *son = son_;

    if (!son || !node)
        return;

    if (node == son)
    {
        son_ = son->get_next();
        return;
    }

    TreeNode *prev_son;
    for (; son && son != node; prev_son = son, son = son->get_next());

    if (son)
        prev_son->set_next(son->get_next());
}

void TreeNode::remove_son(int index)
{
    TreeNode *son = son_;
    for (int i = 0; son && i < index; son = son->get_next(), i++);
    remove_son(son);
}
