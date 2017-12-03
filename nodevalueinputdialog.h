#ifndef NODEVALUEINPUTDIALOG_H
#define NODEVALUEINPUTDIALOG_H

#include <QDialog>

class QLineEdit;

class NodeValueInputDialog : public QDialog
{
    Q_OBJECT

public:
    NodeValueInputDialog(QWidget *parent = 0);
    ~NodeValueInputDialog();
    QString get_value();
    static void set_max_value_length(int length);
    static int get_max_value_length();

private slots:
    void accept();

private:
    QLineEdit *node_value_line_edit_;
    static int max_value_length_;
};

#endif // NODEVALUEINPUTDIALOG_H
