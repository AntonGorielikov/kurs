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

QString TreeNode::get_value() const
{
    return value_;
}

void TreeNode::set_value(const QString &value)
{
    value_ = value;
}

void TreeNode::clear_sons()
{
    if (!son_)
        return;

    TreeNode *prev_son = son_;
    for (TreeNode *son = son_->get_next(); prev_son->get_next();
         (prev_son = son ? : NULL), (son = son ? son->get_next() : NULL))
    {
        delete prev_son;
    }

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

    if (son != node)
        for (; son->get_next() && son->get_next() != node; son = son->get_next());

    TreeNode *son_to_remove = son->get_next();
    son->set_next(son->get_next()->get_next());
    delete son_to_remove;
}

void TreeNode::remove_son(int index)
{
    TreeNode *son = son_;
    for (int i = 0; son && i < index; son = son->get_next(), i++);

    remove_son(son);
}
