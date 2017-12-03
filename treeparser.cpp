#include "treeparser.h"
#include "treenode.h"
#include "default_config.h"
#include <QTextStream>
#include <QFile>

#define ROOT_VALUE "ROOT"

QChar TreeParser::value_opening_char = DEFAULT_VALUE_OPENING_CHAR,
      TreeParser::value_closing_char = DEFAULT_VALUE_CLOSING_CHAR,
      TreeParser::sons_opening_char = DEFAULT_SONS_OPENING_CHAR,
      TreeParser::sons_closing_char = DEFAULT_SONS_CLOSING_CHAR;

QString TreeParser::allowed_chars_between_nodes = DEFAULT_ALLOWED_CHAR_BETWEEN_NODES;

int TreeParser::current_line = 0;

QString TreeParser::error_messages[] = {
    "Unexpected character at line %1",
    "Cannot open file",
    "Unexpected end of file",
    "Empty value at line %1",
    ""
};

int TreeParser::parse_vstf_file(const QString &path, TreeNode *result)
{
    QFile vstf_file(path);

    if (!vstf_file.open(QIODevice::ReadOnly | QIODevice::Text))
        return CANNOT_OPEN_FILE;

    QTextStream in(&vstf_file);

    result->set_parent(NULL);
    result->set_next(NULL);
    result->set_value(ROOT_VALUE);
    result->clear_sons();

    current_line = 1;
    int ret;
    while ((ret = vstf_search_for_branch(&in, result)) == CONTINUE_SONS_SEARCH);

    if (ret == FOUND_CLOSING_SONS_CHAR)
        ret = UNEXPECTED_CHAR;

    vstf_file.close();
    if (ret < 0)
        result->clear_sons();

    return ret;
}

int TreeParser::vstf_search_for_branch(QTextStream *stream, TreeNode *parent_node)
{
    QChar input_char;
    while (!stream->atEnd() &&
           (input_char = stream->read(1).at(0)) != value_opening_char &&
           input_char != sons_closing_char)
    {
        if (!check_char_is_valid(input_char))
            return UNEXPECTED_CHAR;
    }

    if (stream->atEnd())
        return FILE_EOF;
    if (input_char == sons_closing_char)
        return FOUND_CLOSING_SONS_CHAR;

    QString value("");
    while (!stream->atEnd() &&
           (input_char = stream->read(1).at(0)) != value_closing_char)
    {
        value.append(input_char);
    }

    value.replace('\n', "");
    value.replace('\t', "");

    if (value.isEmpty())
        return EMPTY_VALUE;

    TreeNode *new_branch = new TreeNode(parent_node, value);
    return vstf_search_for_sons(stream, new_branch);
}

int TreeParser::vstf_search_for_sons(QTextStream *stream, TreeNode *parent_node)
{
    QChar input_char;
    while (!stream->atEnd() &&
           (input_char = stream->read(1).at(0)) != sons_opening_char &&
           input_char != value_opening_char &&
           input_char != sons_closing_char)
    {
        if (!check_char_is_valid(input_char))
            return UNEXPECTED_CHAR;
    }

    if (stream->atEnd())
        return FILE_EOF;
    if (input_char == sons_closing_char)
        return FOUND_CLOSING_SONS_CHAR;
    if (input_char == value_opening_char)
    {
        stream->seek(stream->pos() - 1);
        return CONTINUE_SONS_SEARCH;
    }

    while (true)
    {
        int ret = vstf_search_for_branch(stream, parent_node);
        switch (ret)
        {
        case CONTINUE_SONS_SEARCH:
            continue;
        case FOUND_CLOSING_SONS_CHAR:
            return CONTINUE_SONS_SEARCH;
        case FILE_EOF:
            return parent_node->get_value() == ROOT_VALUE ?
                (int)FILE_EOF : (int)UNEXPECTED_EOF;
        default:
            return ret;
        }
    }
}

int TreeParser::check_char_is_valid(QChar ch)
{
    if (!allowed_chars_between_nodes.contains(ch))
        return 0;
    else if (ch == '\n')
        current_line++;

    return 1;
}

const QString &TreeParser::get_error_message(int return_value)
{
    static QString err_msg;

    if (return_value > -1 && return_value < ERROR_COUNT - 1)
        return err_msg = "";

    err_msg = error_messages[-return_value - 1];

    if (return_value == EMPTY_VALUE ||
        return_value == UNEXPECTED_CHAR)
    {
        // add additional information for these errors
        err_msg = err_msg.arg(current_line);
    }

    return err_msg;
}

void TreeParser::set_value_opening_char(QChar ch)
{
    value_opening_char = ch;
}

QChar TreeParser::get_value_opening_char()
{
    return value_opening_char;
}

void TreeParser::set_value_closing_char(QChar ch)
{
    value_closing_char = ch;
}

QChar TreeParser::get_value_closing_char()
{
    return value_closing_char;
}

void TreeParser::set_sons_opening_char(QChar ch)
{
    sons_opening_char = ch;
}

QChar TreeParser::get_sons_opening_char()
{
    return sons_opening_char;
}

void TreeParser::set_sons_closing_char(QChar ch)
{
    sons_closing_char = ch;
}

QChar TreeParser::get_sons_closing_char()
{
    return sons_closing_char;
}
