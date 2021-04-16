#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QStorageInfo>
#include <QFileSystemModel>
#include <QTreeView>
#include <QSortFilterProxyModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFileInfoList drives = QDir::drives();

    for (auto& path : drives) {
        qDebug() << path;
        QStorageInfo storage(path.path());

//        qDebug() << storage.rootPath();
//        qDebug() << "name:" << storage.name();
//        qDebug() << "fileSystemType:" << storage.fileSystemType();
//        qDebug() << "size:" << storage.bytesTotal()/1000/1000/1000 << "GB";
//        qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000/1000 << "GB";
//        qDebug() << "\n";

        ui->diskLetter->addItem(path.path());
        ui->diskInfo->setText(((storage.name() != "") ? storage.name() : "Local disk") + " | Free space: " + (QString::number(storage.bytesAvailable()/1024/1024/1024) + " GB of " + QString::number(storage.bytesTotal()/1024/1024/1024) + " GB"));
//        ui->usedSpace->setValue(100 - ((static_cast<float>(storage.bytesAvailable()/1024/1024/1024) / static_cast<float>(storage.bytesTotal()/1024/1024/1024)) * 100));
//        qDebug() << 100 - ((static_cast<float>(storage.bytesAvailable()/1024/1024/1024) / static_cast<float>(storage.bytesTotal()/1024/1024/1024)) * 100);

        // Create new item (top level item)
        QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(ui->tree);
        // Add it on our tree as the top item.
        ui->tree->addTopLevelItem(topLevelItem);
        topLevelItem->setText(0, storage.rootPath());
        // Set text for item
        QString diskName = (storage.name() != "") ? storage.name() : "Local disk";
        topLevelItem->setText(1, diskName + " (" + storage.rootPath() + ")");

//        QDir currentDrive(storage.rootPath());

//        for (auto& subItem : currentDrive.entryList()) {
//            // Create new item and add as child item
//            QTreeWidgetItem *item=new QTreeWidgetItem(topLevelItem);
//            // Set text for item
//            item->setText(1, subItem);
//        }
    }

//    ui->tree->hideColumn(0);
    connect(ui->diskLetter, &QComboBox::currentTextChanged, this, &MainWindow::changeDisk);
    connect(&model, &QFileSystemModel::directoryLoaded, this, &MainWindow::changeDir);
    //connect(ui->treeView, &QAbstractItemView::doubleClicked, ui->treeView, &QTreeView::setRootIndex);

    model.setRootPath("C:/");
    ui->diskLetter->setCurrentText(QDir::rootPath());
//    ui->treeView->setModel(&model);
//    ui->listView->setModel(&model);

    /*ui->treeView->setModel(&model);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setExpandsOnDoubleClick(false);*/

//    ui->path->setText(dir.currentPath());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeDisk(const QString &path)
{
    QStorageInfo storage(path);
    ui->diskInfo->setText(((storage.name() != "") ? storage.name() : "Local disk") + " | Free space: " +             (QString::number(storage.bytesAvailable()/1024/1024/1024) + " GB of " + QString::number(storage.bytesTotal()/1024/1024/1024) + " GB"));
    model.setRootPath(path);
}

void MainWindow::changeDir(const QString &path)
{
//    qDebug() << path;
    ui->path->setText(path);
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{
//    ui->listView->setRootIndex(index);
}

void MainWindow::on_listView_activated(const QModelIndex &index)
{
//    ui->listView->setRootIndex(index);
//    qDebug() << model.rootPath();
//    ui->treeView->setCurrentIndex(index);
//    ui->treeView->expand(index);
}

void MainWindow::on_btnBack_clicked()
{

}

void MainWindow::on_btnForward_clicked()
{

}

void MainWindow::on_btnParent_clicked()
{
    //qDebug() << ui->treeView->currentIndex().parent();
    //ui->treeView->setCurrentIndex(ui->treeView->currentIndex().parent());
//    ui->treeView->setRootIndex(ui->treeView->currentIndex().parent());
}
