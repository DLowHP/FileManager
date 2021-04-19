#include "FileSystemList.h"
#include "ui_FileSystemList.h"

#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QTreeWidgetItem>

FileSystemList::FileSystemList(QString path,  QWidget* parent) :
    QWidget(parent),
    ui(new Ui::FileSystemList)
{
    ui->setupUi(this);

    pathHistory.append(path);
    currentPathIndex = 0;

    QDir currentPath = pathHistory.at(currentPathIndex);
    loadContents(&currentPath);

    ui->tree->setColumnCount(5);

    QTreeWidgetItem* header = new QTreeWidgetItem();
    header->setText(0, "Path");
    header->setText(1, "Name");
    header->setText(2, "Modification date");
    header->setText(3, "Type");
    header->setText(4, "Size");

    ui->tree->setHeaderItem(header);
    //ui->tree->hideColumn(0);

    connect(ui->tree, &QTreeWidget::itemDoubleClicked, this, &FileSystemList::listItemDoubleClicked);
}

FileSystemList::~FileSystemList()
{
    delete ui;
}

void FileSystemList::historyGoBack()
{
    if (currentPathIndex != 0) {
        currentPathIndex--;
        QDir currentPath = pathHistory.at(currentPathIndex);
        loadContents(&currentPath);

        emit historyStatusChange(HistoryStatus::HasFurther);
    }
    if (currentPathIndex == 0) {
        emit historyStatusChange(HistoryStatus::NoPrevious);
    }
}

void FileSystemList::historyGoForward()
{
    if (currentPathIndex < pathHistory.size() - 1) {
        currentPathIndex++;
        QDir currentPath = pathHistory.at(currentPathIndex);
        loadContents(&currentPath);

        emit historyStatusChange(HistoryStatus::HasPrevious);
    }
    if (currentPathIndex == pathHistory.size() - 1) {
        emit historyStatusChange(HistoryStatus::NoFurther);
    }
}

void FileSystemList::changePath(const QString& path)
{
    QDir currentPath(path);

    pathHistory.append(currentPath.path());
    currentPathIndex++;

    loadContents(&currentPath);
    emit historyStatusChange(HistoryStatus::HasPrevious);
}

void FileSystemList::goToParent()
{
    // Deletes further elements from history.
    // Similar to what default Windows File Explorer does.
    pathHistory.resize(currentPathIndex + 1);
    pathHistory.squeeze();

    emit historyStatusChange(HistoryStatus::NoFurther);

    QDir currentPath = pathHistory.at(currentPathIndex);
    currentPath.cdUp();

    pathHistory.append(currentPath.path());
    currentPathIndex++;

    loadContents(&currentPath);
}

void FileSystemList::loadContents(QDir* dir)
{
    ui->tree->clear();
    
    for (auto& item : dir->entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        // Create new item and add to list
        QTreeWidgetItem *treeItem=new QTreeWidgetItem(ui->tree);

        treeItem->setText(0, item.filePath());
        treeItem->setText(1, item.fileName());
        treeItem->setText(2, item.lastModified().toString("yyyy.MM.dd hh:mm"));

        if (item.isDir()) {
            treeItem->setText(3, "File folder");
            treeItem->setText(4, "");
        }
        else {
            treeItem->setText(3, item.completeSuffix());
            treeItem->setText(4, QString::number(item.size()));
        }
    }

    emit pathChanged(dir->path());
}

void FileSystemList::listItemDoubleClicked(QTreeWidgetItem* item)
{
    if (item->text(3) == "File folder") {
        qDebug() << "It's a folder";

        // Deletes further elements from history.
        // Similar to what default Windows File Explorer does.
        if (currentPathIndex < pathHistory.size() - 1) {
            pathHistory.resize(currentPathIndex + 1);
            pathHistory.squeeze();

            emit historyStatusChange(HistoryStatus::NoFurther);
        }

        pathHistory.append(item->text(0));
        currentPathIndex++;

        QDir dir(item->text(0));
        loadContents(&dir);

        emit historyStatusChange(HistoryStatus::HasPrevious);
    }
    else {
        qDebug() << "It's a file";
    }
}