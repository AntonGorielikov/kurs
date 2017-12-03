#include "treemodel.h"
#include "treenode.h"
#include "default_config.h"

TreeModel::TreeModel(const TreeNode **data, QObject *parent) :
    QAbstractItemModel(parent),
    root_(data)
{
}

TreeModel::~TreeModel()
{
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeNode *node = (TreeNode *)index.internalPointer();
    return (node ? node->get_value() : QString(""));
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr(DEFAULT_HEADER_TEXT);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const TreeNode *parent_node;

    if (!parent.isValid())
        parent_node = *root_;
    else
        parent_node = (TreeNode *)parent.internalPointer();

    TreeNode *child_node = parent_node->get_nson(row);
    if (child_node)
        return createIndex(row, column, child_node);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNode *child_node = (TreeNode *)index.internalPointer();
    TreeNode *parent_node = child_node->get_parent();

    if (parent_node == *root_)
        return QModelIndex();

    return createIndex(parent_node->get_index(), 0, parent_node);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    const TreeNode *parent_node;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parent_node = *root_;
    else
        parent_node = (TreeNode *)parent.internalPointer();

    return parent_node->get_nsons();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
