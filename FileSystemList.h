#ifndef FILESYSTEMLIST_H
#define FILESYSTEMLIST_H

#include <QWidget>
#include <QVector>
#include <QDir>
#include <QTreeWidgetItem>

namespace Ui {
class FileSystemList;
}

class FileSystemList : public QWidget
{
    Q_OBJECT

public:
    explicit FileSystemList(QString path, QWidget *parent = nullptr);
    ~FileSystemList();

    enum class HistoryStatus {
        NoPrevious,
        NoFurther,
        HasPrevious,
        HasFurther
    };

    void historyGoBack();
    void historyGoForward();
    void changePath(const QString& path);
    void goToParent();

private:
    Ui::FileSystemList *ui;

    void loadContents(QDir* dir);

    QVector<QString> pathHistory;
    int currentPathIndex;

private slots:
    void listItemDoubleClicked(QTreeWidgetItem* item);

signals:
    void pathChanged(const QString& path);
    void diskChanged(const QString& path);
    void historyStatusChange(HistoryStatus status);
};

#endif // FILESYSTEMLIST_H
