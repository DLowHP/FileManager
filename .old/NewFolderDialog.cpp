#include "NewFolderDialog.h"
#include "ui_NewFolderDialog.h"

NewFolderDialog::NewFolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFolderDialog)
{
    ui->setupUi(this);

    connect(ui->btnOK, &QPushButton::clicked, this, &NewFolderDialog::btnOK);
    connect(ui->btnCancel, &QPushButton::clicked, this, &NewFolderDialog::btnCancel);
}

NewFolderDialog::~NewFolderDialog()
{
    delete ui;
}

void NewFolderDialog::btnOK()
{
    emit folderName(ui->fileName->text());
    this->close();
}

void NewFolderDialog::btnCancel()
{
    this->close();
}
