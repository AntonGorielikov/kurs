#include "treefilewriter.h"
#include "treenode.h"
#include "treeparser.h"
#include "default_config.h"

#include <QFile>
#include <QTextStream>
#include <QTextStream>

int TreeFileWriter::write_vstf_file(const TreeNode *root, const QString &path)
{
    QFile vstf_file(path);

    if (!vstf_file.open(QIODevice::WriteOnly | QIODevice::Text))
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
        *stream << offset << TreeParser::get_value_opening_char()
            << node->get_value() << TreeParser::get_value_closing_char() << endl;
        if (son)
            *stream << offset << TreeParser::get_sons_opening_char() << endl;
    }

    for (; son; son = son->get_next())
        write_vstf_line(stream, son, level + 1);

    if (level >= 0 && node->get_son())
        *stream << offset << TreeParser::get_sons_closing_char() << endl;
}
