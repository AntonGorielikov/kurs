#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QSpinBox;

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

private slots:
    void accept();

private:
    QLabel *value_opening_label_;
    QLineEdit *value_opening_line_edit_;
    QLabel *value_closing_label_;
    QLineEdit *value_closing_line_edit_;
    QLabel *sons_opening_label_;
    QLineEdit *sons_opening_line_edit_;
    QLabel *sons_closing_label_;
    QLineEdit *sons_closing_line_edit_;
    QLabel *max_value_length_label_;
    QSpinBox *max_value_length_spin_box_;
};

#endif // OPTIONSDIALOG_H
