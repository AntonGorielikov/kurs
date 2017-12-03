#include "nodevalueinputdialog.h"
#include "treeparser.h"
#include "default_config.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QRegExpValidator>
#include <QDialogButtonBox>
#include <QMessageBox>

int NodeValueInputDialog::max_value_length_ = DEFAULT_MAX_VALUE_LENGTH;

NodeValueInputDialog::NodeValueInputDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setAttribute(Qt::WA_QuitOnClose, false);

    QVBoxLayout *main_layout = new QVBoxLayout;

    main_layout->addWidget(new QLabel(tr("Enter node value")));

    node_value_line_edit_ = new QLineEdit();
    node_value_line_edit_->setPlaceholderText(tr("Value"));
    main_layout->addWidget(node_value_line_edit_);

    QChar value_op = TreeParser::get_value_opening_char(),
          value_cl = TreeParser::get_value_closing_char();

    QString reg_exp = QString("[^") +
        (value_op.isLetterOrNumber() || value_op.isSpace() ? "" : "\\") + value_op +
        QString("^") + (value_cl.isLetterOrNumber() || value_cl.isSpace() ? "" : "\\") + value_cl +
        QString("]{,") + QString::number(max_value_length_) + '}';
    QRegExpValidator *validator = new QRegExpValidator(QRegExp(reg_exp));
    node_value_line_edit_->setValidator(validator);

    QDialogButtonBox *button_box = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel);
    main_layout->addWidget(button_box);
    setLayout(main_layout);

    connect(button_box, &QDialogButtonBox::accepted,
        this, &NodeValueInputDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected,
        this, &NodeValueInputDialog::reject);
}

NodeValueInputDialog::~NodeValueInputDialog()
{
}

QString NodeValueInputDialog::get_value()
{
    return node_value_line_edit_->text();
}

void NodeValueInputDialog::set_max_value_length(int length)
{
    max_value_length_ = length;
}

int NodeValueInputDialog::get_max_value_length()
{
    return max_value_length_;
}

void NodeValueInputDialog::accept()
{
    if (node_value_line_edit_->text().isEmpty())
        QMessageBox::warning(this, tr("Warning"), tr("Value is empty."), QMessageBox::Ok);
    else
        QDialog::accept();
}
