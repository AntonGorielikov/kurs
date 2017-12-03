#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTreeView;
class TreeModel;
class TreeNode;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void new_file();
    void open();
    void save();
    void save_as();
    void exit();
    void options();
    void about();
    void about_qt();
    void remove_node();
    void add_node_son();
    void add_root_son();
    void on_custom_context_menu(const QPoint &point);

private:
    void create_actions();
    void create_menus();
    // returns 1 if you can proceed
    // 0 otherwise
    int do_you_want_to_save_file();
    // do hard reset on tree
    // required when removing nodes to clear wild pointers
    void tree_reset();

    QTreeView *tree_interface_;
    TreeModel *tree_model_;
    TreeNode *root_node_;

    QMenu *tree_item_context_menu_;
    QAction *add_node_son_action_;
    QAction *remove_node_action_;

    QMenu *tree_context_menu_;
    QAction *add_root_son_;

    QMenu *file_menu_;
    QAction *new_action_;
    QAction *open_action_;
    QAction *save_action_;
    QAction *save_as_action_;
    QAction *exit_action_;

    QMenu *tools_menu_;
    QAction *options_action_;

    QMenu *about_menu_;
    QAction *about_programm_action_;
    QAction *about_qt_action_;

    int is_dirty_;
    QString file_path_;
};

#endif // MAINWINDOW_H
