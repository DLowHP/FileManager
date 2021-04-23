#ifndef NEWFOLDERDIALOG_H
#define NEWFOLDERDIALOG_H

#include <QDialog>

namespace Ui {
class NewFolderDialog;
}

class NewFolderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFolderDialog(QWidget *parent = nullptr);
    ~NewFolderDialog();

private:
    Ui::NewFolderDialog *ui;

private slots:
    void btnOK();
    void btnCancel();

signals:
    void folderName(const QString& name);
};

#endif // NEWFOLDERDIALOG_H
