#include "treefilewriter.h"
#include "treenode.h"
#include <QFile>
#include <QTextStream>
#include <QTextStream>

int TreeFileWriter::write_vstf_file(const TreeNode *root, const QString &path)
{
    QFile vstf_file(path);

    if (!vstf_file.open(QIODevice::WriteOnly))
        return -1;

    QTextStream stream(&vstf_file);
    write_vstf_line(&stream, root, -1);
    vstf_file.close();
    return 0;
}

void TreeFileWriter::write_vstf_line(QTextStream *stream, const TreeNode *node, int level)
{
    const TreeNode *son = node->get_son();
    QString offset("");
    for (int i = 0; i < level; i++)
        offset.append("    ");

    if (level >= 0)
    {
        *stream << offset << '[' << node->get_value() << ']' << endl;
        if (son)
            *stream << offset << '{' << endl;
    }

    for (; son; son = son->get_next())
        write_vstf_line(stream, son, level + 1);

    if (level >= 0 && node->get_son())
        *stream << offset << '}' << endl;
}
