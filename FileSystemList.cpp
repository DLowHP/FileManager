#include "FileSystemList.h"
#include "ui_FileSystemList.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QDateTime>
#include <QTreeWidgetItem>
#include <QDesktopServices>
#include <QProcess>

#include "TextInputDialog.h"

FileSystemList::FileSystemList(QString path,  QWidget* parent) :
    QWidget(parent),
    ui(new Ui::FileSystemList)
{
    ui->setupUi(this);

    pathHistory.append(path);
    currentPathIndex = 0;

    clipboard = "";
    clipboardStatus = ClipboardStatus::None;

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

void FileSystemList::copy()
{
    if (ui->tree->currentItem()) {
        clipboard = ui->tree->currentItem()->text(0);
        clipboardStatus = ClipboardStatus::Copy;
        qDebug() << clipboard;
    }
}

void FileSystemList::cut()
{
    if (ui->tree->currentItem()) {
        clipboard = ui->tree->currentItem()->text(0);
        clipboardStatus = ClipboardStatus::Cut;
        qDebug() << clipboard;
    }
}

void FileSystemList::paste()
{
    if (clipboard != "") {
        switch (clipboardStatus) {
        case ClipboardStatus::Copy: {
            QFileInfo file(clipboard);
            //QString pathB(pathHistory[currentPathIndex] + file.fileName());
            QFile::copy(clipboard, pathHistory[currentPathIndex] + "/" + file.fileName());
            qDebug() << clipboard << "\n";
            qDebug() << pathHistory[currentPathIndex] + "/" + file.fileName();
            break;
        }
        case ClipboardStatus::Cut: {
            QFileInfo file(clipboard);
            //QString pathB(pathHistory[currentPathIndex] + file.fileName());
            QFile::copy(clipboard, pathHistory[currentPathIndex] + "/" + file.fileName());

            QFile oldFile(clipboard);
            oldFile.moveToTrash();
            break;
        }
        case ClipboardStatus::None: break;
        }

        QDir current(pathHistory[currentPathIndex]);
        loadContents(&current);
    }
}

void FileSystemList::newFolder()
{
    TextInputDialog* d = new TextInputDialog();
    d->setLabel("Enter folder name: ");
    connect(d, &TextInputDialog::text, this, &FileSystemList::createNewFolder);
    d->exec();
}

void FileSystemList::createNewFolder(const QString& name)
{
    if (name != "") {
        QDir current(pathHistory[currentPathIndex]);
        current.mkdir(name);
        loadContents(&current);
    }
}

void FileSystemList::renameItem()
{
    qDebug() << "Rename item";

    if (ui->tree->currentItem()) {
        TextInputDialog* d = new TextInputDialog();
        d->setLabel("Enter new item name: ");
        d->setInputText(ui->tree->currentItem()->text(0));
        connect(d, &TextInputDialog::text, this, &FileSystemList::setNewName);
        d->exec();
    }
}

void FileSystemList::newTextFile()
{
    QDir current(pathHistory[currentPathIndex]);
    TextInputDialog* d = new TextInputDialog();
    d->setLabel("Enter new text file name: ");
    d->setInputText(".txt");
    connect(d, &TextInputDialog::text, this, &FileSystemList::createNewFile);
    d->exec();
}

void FileSystemList::newOtherFile()
{
    QDir current(pathHistory[currentPathIndex]);
    TextInputDialog* d = new TextInputDialog();
    d->setLabel("Enter new text file name: ");
    d->setInputText(".txt");
    connect(d, &TextInputDialog::text, this, &FileSystemList::createNewFile);
    d->exec();
}

void FileSystemList::createNewFile(const QString& name)
{
    if (name != "") {
        QFile file(pathHistory[currentPathIndex] + "/" + name);
        file.open(QIODevice::WriteOnly);

        QDir current(pathHistory[currentPathIndex]);
        loadContents(&current);
    }
}

void FileSystemList::openCmd()
{
    //cmd.startDetached("cmd", QStringList() << "/K \"cd /d '" + pathHistory[currentPathIndex] + "'\"");
    //QProcess::startDetached("cmd /K cmd /K \"cd /d " + pathHistory[currentPathIndex] + "'\"");
    QProcess::startDetached("cmd", QStringList() << "/K \"cd /d '" + pathHistory[currentPathIndex] + "'\"", pathHistory[currentPathIndex]);
}

void FileSystemList::openPowerShell()
{
    QProcess::startDetached("powershell", QStringList() << "-NoExit -command \"cd /d '" + pathHistory[currentPathIndex] + "'\"", pathHistory[currentPathIndex]);
}

void FileSystemList::setNewName(const QString& name)
{
    if (name != "") {
        QDir item(ui->tree->currentItem()->text(0));
        item.rename(ui->tree->currentItem()->text(0), name);

        // Reload dir
        QDir current(pathHistory[currentPathIndex]);
        loadContents(&current);
    }
}

void FileSystemList::deleteItem()
{
    if (ui->tree->currentItem()) {
        QFile file(ui->tree->currentItem()->text(0));
        file.moveToTrash();
        QDir current(pathHistory[currentPathIndex]);
        loadContents(&current);
    }
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

        QUrl path(item->text(0));
        QDesktopServices::openUrl(path);
    }
}