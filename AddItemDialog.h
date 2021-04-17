#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H
#include <DataManager.h>
#include <QDialog>

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = 0);
    ~AddItemDialog();
    BimInfo getBimInfo();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

public:
    Ui::AddItemDialog *ui;
};

#endif // ADDITEMDIALOG_H
