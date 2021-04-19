#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QStorageInfo>
#include <QFileSystemModel>
#include <QTreeView>
#include <QSortFilterProxyModel>

#include "FileSystemList.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFileInfoList drives = QDir::drives();
    for (auto& path : drives) {
        ui->diskLetter->addItem(path.path());
    }

    fslist = new FileSystemList("C:/");
    ui->fileSystemListContainer->insertWidget(0, fslist);
    ui->fileSystemListContainer->setCurrentIndex(0);

    ui->diskLetter->setCurrentText("C:/");
    
    connect(ui->btnChangePath, &QPushButton::clicked, this, &MainWindow::btnChangePath);

    connect(ui->diskLetter, &QComboBox::textActivated, this, &MainWindow::changePath);
    connect(fslist, &FileSystemList::pathChanged, this, &MainWindow::updatePath);
    connect(fslist, &FileSystemList::historyStatusChange, this, &MainWindow::listHistoryStatusChange);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBack_clicked()
{
    fslist->historyGoBack();
}

void MainWindow::on_btnForward_clicked()
{
    fslist->historyGoForward();
}

void MainWindow::on_btnParent_clicked()
{
    fslist->goToParent();
}

void MainWindow::btnChangePath()
{
    fslist->changePath(ui->path->text());
}

void MainWindow::changePath(const QString& path)
{
    fslist->changePath(path);
    ui->path->setText(path);
    QStorageInfo storage(path);
    ui->diskInfo->setText(((storage.name() != "") ? storage.name() : "Local disk") + " | Free space: " + (QString::number(storage.bytesAvailable() / 1024 / 1024 / 1024) + " GB of " + QString::number(storage.bytesTotal() / 1024 / 1024 / 1024) + " GB"));
}

void MainWindow::updatePath(const QString &path)
{
    ui->path->setText(path);

    QString diskLetter = path.left(3);

    if (diskLetter.compare(ui->diskLetter->currentText(), Qt::CaseInsensitive) != 0) {
        QStorageInfo storage(diskLetter);
        ui->diskLetter->setCurrentText(diskLetter);
        ui->diskInfo->setText(((storage.name() != "") ? storage.name() : "Local disk") + " | Free space: " + (QString::number(storage.bytesAvailable() / 1024 / 1024 / 1024) + " GB of " + QString::number(storage.bytesTotal() / 1024 / 1024 / 1024) + " GB"));
    }
}

void MainWindow::listHistoryStatusChange(FileSystemList::HistoryStatus status)
{
    switch (status) {
    case FileSystemList::HistoryStatus::NoPrevious: {
        ui->btnBack->setDisabled(true);
        break;
    }
    case FileSystemList::HistoryStatus::NoFurther: {
        ui->btnForward->setDisabled(true);
        break;
    }
    case FileSystemList::HistoryStatus::HasPrevious: {
        ui->btnBack->setDisabled(false);
        break;
    }
    case FileSystemList::HistoryStatus::HasFurther: {
        ui->btnForward->setDisabled(false);
        break;
    }
    }
}
