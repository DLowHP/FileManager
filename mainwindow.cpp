#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QStorageInfo>
#include <QFileSystemModel>
#include <QTreeView>
#include <QSortFilterProxyModel>
#include <QApplication>

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
    connect(ui->btnCopy, &QPushButton::clicked, fslist, &FileSystemList::copy);
    connect(ui->btnMove, &QPushButton::clicked, fslist, &FileSystemList::cut);
    connect(ui->btnPaste, &QPushButton::clicked, fslist, &FileSystemList::paste);
    connect(ui->btnNewFolder, &QPushButton::clicked, fslist, &FileSystemList::newFolder);

    // File menu
    //connect(ui->actionNewWindow, &QAction::triggered, this, &MainWindow::openNewWindow);
    connect(ui->actionOpenCmd, &QAction::triggered, fslist, &FileSystemList::openCmd);
    connect(ui->actionOpenPowerShell, &QAction::triggered, fslist, &FileSystemList::openPowerShell);
    //connect(ui->actionHelp, &QAction::triggered, fslist, &MainWindow::openHelp);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::closeApp);

    // Home menu
    connect(ui->actionCopy, &QAction::triggered, fslist, &FileSystemList::copy);
    connect(ui->actionCut, &QAction::triggered, fslist, &FileSystemList::cut);
    connect(ui->actionPaste, &QAction::triggered, fslist, &FileSystemList::paste);
    connect(ui->actionDelete, &QAction::triggered, fslist, &FileSystemList::deleteItem);
    connect(ui->actionRename, &QAction::triggered, fslist, &FileSystemList::renameItem);
    connect(ui->actionNewFolder, &QAction::triggered, fslist, &FileSystemList::newFolder);
        // New item submenu
    connect(ui->actionTextFile, &QAction::triggered, fslist, &FileSystemList::newTextFile);
    connect(ui->actionOtherFormat, &QAction::triggered, fslist, &FileSystemList::newOtherFile);

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

void MainWindow::closeApp()
{
    QApplication::exit(0);
}
