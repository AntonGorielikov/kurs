#include "mainwindow.h"
#include "treemodel.h"
#include "treenode.h"
#include "treeparser.h"
#include "treefilewriter.h"
#include "nodevalueinputdialog.h"
#include "optionsdialog.h"

#include <QTreeView>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      is_dirty_(0),
      file_path_("")
{
    root_node_ = new TreeNode;
    tree_interface_ = new QTreeView;
    tree_model_ = new TreeModel((const TreeNode **)&root_node_);
    tree_interface_->setModel(tree_model_);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(tree_interface_);

    QWidget *main_widget = new QWidget;
    main_widget->setLayout(main_layout);
    setCentralWidget(main_widget);

    create_actions();
    create_menus();

    setWindowTitle(tr("Untitled"));
    tree_interface_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree_interface_, &QTreeView::customContextMenuRequested,
        this, &MainWindow::on_custom_context_menu);
}

MainWindow::~MainWindow()
{
    delete root_node_;
}

void MainWindow::new_file()
{
    if (!do_you_want_to_save_file())
        return;

    root_node_->clear_sons();
    file_path_ = "";
    setWindowTitle(tr("Untitled"));
    emit tree_model_->layoutChanged();
}

void MainWindow::open()
{
    if (!do_you_want_to_save_file())
        return;

    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::homePath(), tr("TreeFile (*.vstf)"));
    if (file_name.isEmpty())
        return;

    TreeNode *temp_root = new TreeNode;
    int ret = TreeParser::parse_vstf_file(file_name, temp_root);

    if (ret < 0)
    {
        QMessageBox::warning(this, tr("Error"), tr("Error while opening %1 file\n%2")
            .arg(file_name).arg(TreeParser::get_error_message(ret)));
        delete temp_root;
        return;
    }

    delete root_node_;
    root_node_ = temp_root;
    file_path_ = file_name;
    setWindowTitle(file_path_);
    tree_reset();
}

void MainWindow::save()
{
    if (!is_dirty_)
        return;

    if (file_path_.isEmpty())
        save_as();
    else if (TreeFileWriter::write_vstf_file(root_node_, file_path_))
    {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file for writing"));
        return;
    }
    is_dirty_ = 0;
}

void MainWindow::save_as()
{
    file_path_ = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QDir::homePath(),
                               tr("TreeFile (*.vstf)"));
    if (file_path_.isEmpty())
        return;

    if (TreeFileWriter::write_vstf_file(root_node_, file_path_))
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file for writing"));

    setWindowTitle(file_path_);
    is_dirty_ = 0;
}

void MainWindow::exit()
{
    if (do_you_want_to_save_file())
        close();
}

void MainWindow::options()
{
    OptionsDialog().exec();
}

void MainWindow::about()
{
    QMessageBox::information(this, tr("About"),
        tr("This programm was in scope of course project\n"
        "from Object-Oriented Programming subject by:\n"
        "student of Ukrainan National University Lviv Polytechnic\n"
        "Institute of Computer Science and Information Technology\n"
        "Faculty of Software Engineering\n"
        "Group PZ-23\n"
        "Anton Gorielikov"));
}

void MainWindow::about_qt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_custom_context_menu(const QPoint &point)
{
    if (tree_interface_->indexAt(point).isValid())
        tree_item_context_menu_->exec(tree_interface_->mapToGlobal(point));
    else
        tree_context_menu_->exec(tree_interface_->mapToGlobal(point));
}

#include <QDebug>
void print_tree(TreeNode *root, int level)
{
    QString offset("");
    for (int i = 0; i < level; i++)
        offset.append("    ");

    qDebug() << offset << root->get_value();
    for (TreeNode *son = root->get_son(); son; son = son->get_next())
        print_tree(son, level + 1);
}

void MainWindow::remove_node()
{
    TreeNode *node_to_remove = (TreeNode *)tree_interface_->currentIndex().internalPointer();
    node_to_remove->get_parent()->remove_son(node_to_remove);
    delete node_to_remove;
    tree_reset();
    is_dirty_ = 1;
}

void MainWindow::add_node_son()
{
    NodeValueInputDialog dialog;
    if (dialog.exec() != QDialog::Accepted)
        return;

    TreeNode *parent_node = (TreeNode *)tree_interface_->currentIndex().internalPointer();
    new TreeNode(parent_node, dialog.get_value());
    emit tree_model_->layoutChanged();
    is_dirty_ = 1;
}

void MainWindow::add_root_son()
{
    NodeValueInputDialog dialog;
    if (dialog.exec() != QDialog::Accepted)
        return;

    new TreeNode(root_node_, dialog.get_value());
    emit tree_model_->layoutChanged();
    is_dirty_ = 1;
}

void MainWindow::create_actions()
{
    new_action_ = new QAction(tr("&New"), this);
    new_action_->setShortcut(QKeySequence::New);
    new_action_->setStatusTip(tr("Create new file"));
    connect(new_action_, &QAction::triggered, this, &MainWindow::new_file);

    open_action_ = new QAction(tr("&Open"), this);
    open_action_->setShortcut(QKeySequence::Open);
    open_action_->setStatusTip(tr("Open file"));
    connect(open_action_, &QAction::triggered, this, &MainWindow::open);

    save_action_ = new QAction(tr("&Save"), this);
    save_action_->setShortcut(QKeySequence::Save);
    save_action_->setStatusTip(tr("Save file"));
    connect(save_action_, &QAction::triggered, this, &MainWindow::save);

    save_as_action_ = new QAction(tr("Save As"), this);
    save_as_action_->setShortcut(QKeySequence::SaveAs);
    save_as_action_->setStatusTip(tr("Save file to specific location"));
    connect(save_as_action_, &QAction::triggered, this, &MainWindow::save_as);

    exit_action_ = new QAction(tr("E&xit"), this);
    exit_action_->setShortcut(QKeySequence::Quit);
    exit_action_->setStatusTip(tr("Exit programm"));
    connect(exit_action_, &QAction::triggered, this, &MainWindow::exit);

    options_action_ = new QAction(tr("&Options"), this);
    options_action_->setStatusTip(tr("Configure programm"));
    connect(options_action_, &QAction::triggered, this, &MainWindow::options);

    about_programm_action_ = new QAction(tr("About &Programm"), this);
    about_programm_action_->setStatusTip(tr("Show info about programm"));
    connect(about_programm_action_, &QAction::triggered, this, &MainWindow::about);

    about_qt_action_ = new QAction(tr("About &Qt"), this);
    about_qt_action_->setStatusTip(tr("Show info about Qt"));
    connect(about_qt_action_, &QAction::triggered, this, &MainWindow::about_qt);

    add_node_son_action_ = new QAction(tr("Add new son"));
    connect(add_node_son_action_, &QAction::triggered,
        this, &MainWindow::add_node_son);

    remove_node_action_ = new QAction(tr("Remove node"));
    connect(remove_node_action_, &QAction::triggered,
        this, &MainWindow::remove_node);

    add_root_son_ = new QAction(tr("Add new branch"));
    connect(add_root_son_, &QAction::triggered,
        this, &MainWindow::add_root_son);
}

void MainWindow::create_menus()
{
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(new_action_);
    file_menu_->addAction(open_action_);
    file_menu_->addAction(save_action_);
    file_menu_->addAction(save_as_action_);
    file_menu_->addSeparator();
    file_menu_->addAction(exit_action_);

    tools_menu_ = menuBar()->addMenu(tr("&Tools"));
    tools_menu_->addAction(options_action_);

    about_menu_ = menuBar()->addMenu(tr("&About"));
    about_menu_->addAction(about_programm_action_);
    about_menu_->addAction(about_qt_action_);

    tree_context_menu_ = new QMenu;
    tree_context_menu_->addAction(add_root_son_);

    tree_item_context_menu_ = new QMenu;
    tree_item_context_menu_->addAction(add_node_son_action_);
    tree_item_context_menu_->addAction(remove_node_action_);
}

int MainWindow::do_you_want_to_save_file()
{
    if (!is_dirty_)
        return 1;

    QMessageBox msg_box;
    msg_box.setText("The document has been modified.");
    msg_box.setInformativeText("Do you want to save your changes?");
    msg_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msg_box.setDefaultButton(QMessageBox::Save);
    int ret = msg_box.exec();

    switch (ret)
    {
    case QMessageBox::Save:
        if (file_path_.isEmpty())
        {
            file_path_ = QFileDialog::getSaveFileName(this, tr("Save File"),
                QDir::homePath(), tr("TreeFile (*.vstf)"));

            if (file_path_.isEmpty())
                return 0;
        }
        if (TreeFileWriter::write_vstf_file(root_node_, file_path_))
        {
            QMessageBox::warning(this, tr("Error"), tr("Cannot open file for writing"));
            return 0;
        }
    case QMessageBox::Discard:
        is_dirty_ = 0;
        return 1;
        break;
    case QMessageBox::Cancel:
    default:
        return 0;
    }
}

void MainWindow::tree_reset()
{
    TreeModel *new_model = new TreeModel((const TreeNode **)&root_node_, this);
    tree_interface_->setModel(new_model);
    delete tree_model_;
    tree_model_ = new_model;
}
