#ifndef FILESYSTEMLIST_H
#define FILESYSTEMLIST_H

#include <QWidget>
#include <QVector>
#include <QDir>
#include <QTreeWidgetItem>
#include <QProcess>

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

public slots:
    void copy();
    void cut();
    void paste();
    void newFolder();
    void deleteItem();
    void renameItem();
    void newTextFile();
    void newOtherFile();
    void openCmd();
    void openPowerShell();

private:
    Ui::FileSystemList *ui;

    void loadContents(QDir* dir);

    QVector<QString> pathHistory;
    int currentPathIndex;

    QString clipboard;
    enum class ClipboardStatus {
        None,
        Copy,
        Cut
    };
    ClipboardStatus clipboardStatus;

private slots:
    void listItemDoubleClicked(QTreeWidgetItem* item);
    void createNewFolder(const QString& name);
    void setNewName(const QString& name);
    void createNewFile(const QString& name);

signals:
    void pathChanged(const QString& path);
    void diskChanged(const QString& path);
    void historyStatusChange(HistoryStatus status);
};

#endif // FILESYSTEMLIST_H
