#ifndef TREEFILEWRITER_H
#define TREEFILEWRITER_H

#include <QString>

class TreeNode;
class QTextStream;

class TreeFileWriter
{
public:
    static int write_vstf_file(const TreeNode *root, const QString &path);

private:
    static void write_vstf_line(QTextStream *stream, const TreeNode *node, int level);
};

#endif // TREEFILEWRITER_H
