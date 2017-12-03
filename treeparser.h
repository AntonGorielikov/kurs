#ifndef TREEPARSER_H
#define TREEPARSER_H

#include <QString>
#include <QChar>

class TreeNode;
class QTextStream;

class TreeParser
{
public:
    // parse file with *.vstf format
    // on success return 1
    // on failure return < 0
    static int parse_vstf_file(const QString &path, TreeNode *result);
    // use to get meaningfull error message
    // if parsing failure occurs
    static const QString &get_error_message(int return_value);

    static void set_value_opening_char(QChar ch);
    static QChar get_value_opening_char();
    static void set_value_closing_char(QChar ch);
    static QChar get_value_closing_char();
    static void set_sons_opening_char(QChar ch);
    static QChar get_sons_opening_char();
    static void set_sons_closing_char(QChar ch);
    static QChar get_sons_closing_char();

private:
    enum tree_parser_states {
        FILE_EOF = 1,
        FOUND_CLOSING_SONS_CHAR = 2,
        CONTINUE_SONS_SEARCH = 3,
    };
    enum tree_parser_errors {
        UNEXPECTED_CHAR = -1,
        CANNOT_OPEN_FILE = -2,
        UNEXPECTED_EOF = -3,
        EMPTY_VALUE = -4,
        ERROR_COUNT
    };
    static QChar value_opening_char,
                 value_closing_char,
                 sons_opening_char,
                 sons_closing_char;
    static QString allowed_chars_between_nodes;
    static QString error_messages[-ERROR_COUNT + 2];
    static int current_line;

    static int vstf_search_for_branch(QTextStream *stream, TreeNode *parent_node);
    static int vstf_search_for_sons(QTextStream *stream, TreeNode *parent_node);

    static int check_char_is_valid(QChar ch);
};

#endif // TREEPARSER_H
