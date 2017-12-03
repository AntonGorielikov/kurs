#include "optionsdialog.h"
#include "treeparser.h"
#include "nodevalueinputdialog.h"
#include "default_config.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QGridLayout>
#include <QMessageBox>

OptionsDialog::OptionsDialog(QWidget *parent)
    : QDialog(parent)
{
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("[\\S\\d\\w]"));
    QGridLayout *main_layout = new QGridLayout;

    value_opening_label_ = new QLabel(tr("Value opening character:"));
    value_opening_line_edit_ = new QLineEdit(TreeParser::get_value_opening_char());
    value_opening_line_edit_->setValidator(validator);
    main_layout->addWidget(value_opening_label_, 0, 0, 1, 1);
    main_layout->addWidget(value_opening_line_edit_, 0, 1, 1, 1);

    value_closing_label_ = new QLabel(tr("Value closing character:"));
    value_closing_line_edit_ = new QLineEdit(TreeParser::get_value_closing_char());
    value_closing_line_edit_->setValidator(validator);
    main_layout->addWidget(value_closing_label_, 0, 2, 1, 1);
    main_layout->addWidget(value_closing_line_edit_, 0, 3, 1, 1);

    sons_opening_label_ = new QLabel(tr("Sons opening character:"));
    sons_opening_line_edit_ = new QLineEdit(TreeParser::get_sons_opening_char());
    sons_opening_line_edit_->setValidator(validator);
    main_layout->addWidget(sons_opening_label_, 1, 0, 1, 1);
    main_layout->addWidget(sons_opening_line_edit_, 1, 1, 1, 1);

    sons_closing_label_ = new QLabel(tr("Sons closing character:"));
    sons_closing_line_edit_ = new QLineEdit(TreeParser::get_sons_closing_char());
    sons_closing_line_edit_->setValidator(validator);
    main_layout->addWidget(sons_closing_label_, 1, 2, 1, 1);
    main_layout->addWidget(sons_closing_line_edit_, 1, 3, 1, 1);

    max_value_length_label_ = new QLabel(tr("Max value length:"));
    max_value_length_spin_box_ = new QSpinBox;
    max_value_length_spin_box_->setMinimum(MIN_MAX_VALUE_LENGTH_VALUE);
    max_value_length_spin_box_->setMaximum(MAX_MAX_VALUE_LENGTH_VALUE);
    max_value_length_spin_box_->setValue(NodeValueInputDialog::get_max_value_length());
    main_layout->addWidget(max_value_length_label_, 2, 0, 1, 1);
    main_layout->addWidget(max_value_length_spin_box_, 2, 1, 1, 1);

    QDialogButtonBox *button_box = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel);
    main_layout->addWidget(button_box, 2, 3, 1, 1);

    setLayout(main_layout);

    connect(button_box, &QDialogButtonBox::accepted, this, &OptionsDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &OptionsDialog::reject);
}

OptionsDialog::~OptionsDialog()
{
}

void OptionsDialog::accept()
{
    QChar value_op = value_opening_line_edit_->text().at(0),
        value_cl = value_closing_line_edit_->text().at(0),
        sons_op = sons_opening_line_edit_->text().at(0),
        sons_cl = sons_closing_line_edit_->text().at(0);

    if (value_op == value_cl || value_op == sons_op || value_op == sons_cl ||
        value_cl == sons_op || value_cl == sons_cl || sons_op == sons_cl)
    {
        QMessageBox::warning(this, tr("Warning"),
            tr("Information that you entered contains repeating characters"));
        return;
    }

    TreeParser::set_value_opening_char(value_op);
    TreeParser::set_value_closing_char(value_cl);
    TreeParser::set_sons_opening_char(sons_op);
    TreeParser::set_sons_closing_char(sons_cl);
    NodeValueInputDialog::set_max_value_length(max_value_length_spin_box_->value());
    QDialog::accept();
}
