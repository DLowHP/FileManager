#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include <QDir>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QDir dir;
    QFileSystemModel model;

private slots:
    void changeDisk(const QString &path);
    void changeDir(const QString &path);
    void on_treeView_activated(const QModelIndex &index);
    void on_listView_activated(const QModelIndex &index);
    void on_btnBack_clicked();
    void on_btnForward_clicked();
    void on_btnParent_clicked();
};
#endif // MAINWINDOW_H
