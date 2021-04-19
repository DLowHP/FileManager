#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include <QDir>
#include <QFileSystemModel>

#include "FileSystemList.h"

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
    FileSystemList* fslist;

private slots:
    void on_btnBack_clicked();
    void on_btnForward_clicked();
    void on_btnParent_clicked();
    void btnChangePath();
    void changePath(const QString &path);
    void updatePath(const QString& path);
    void listHistoryStatusChange(FileSystemList::HistoryStatus status);
};
#endif // MAINWINDOW_H
