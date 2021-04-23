#include "TextInputDialog.h"
#include "ui_TextInputDialog.h"

TextInputDialog::TextInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextInputDialog)
{
    ui->setupUi(this);

    connect(ui->btnOK, &QPushButton::clicked, this, &TextInputDialog::btnOK);
    connect(ui->btnCancel, &QPushButton::clicked, this, &TextInputDialog::btnCancel);
}

TextInputDialog::~TextInputDialog()
{
    delete ui;
}

void TextInputDialog::setLabel(const QString &text)
{
    ui->label->setText(text);
}

void TextInputDialog::setInputText(const QString& text)
{
    ui->input->setText(text);
}

void TextInputDialog::btnOK()
{
    emit text(ui->input->text());
    this->close();
}

void TextInputDialog::btnCancel()
{
    this->close();
}
