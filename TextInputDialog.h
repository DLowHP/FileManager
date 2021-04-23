#ifndef TEXTINPUTDIALOG_H
#define TEXTINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class TextInputDialog;
}

class TextInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextInputDialog(QWidget *parent = nullptr);
    ~TextInputDialog();

    void setLabel(const QString& text);
    void setInputText(const QString& text);

private:
    Ui::TextInputDialog *ui;

private slots:
    void btnOK();
    void btnCancel();

signals:
    void text(const QString& name);
};

#endif // TEXTINPUTDIALOG_H
