#include "mainwindow.h"
#include <QApplication>

#include "treeparser.h"
#include "treefilewriter.h"
#include "treenode.h"

#include <QDebug>

void print_tree(int level, TreeNode *son)
{
    QString output("");
    for(int i = 0; i < level; i++)
        output.append("    ");

    qDebug() << output.append(son->get_value());

    for (TreeNode *node = son->get_son(); node; node = node->get_next())
        print_tree(level + 1, node);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TreeNode *root = new TreeNode;
    int ret = TreeParser::parse_vstf_file("/home/antongorielikov/personal/tree.vstf", root);

    if(ret < 0)
        qDebug() << TreeParser::get_error_message(ret);
    else
    {
        print_tree(0, root);
        TreeFileWriter::write_vstf_file(root, "output.vstf");
    }

    return a.exec();
}
