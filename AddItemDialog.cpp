#include "AddItemDialog.h"
#include "ui_AddItemDialog.h"
#include <QDateTime>

AddItemDialog::AddItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);
}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}


BimInfo AddItemDialog::getBimInfo(){
    BimInfo info;
    info.num = ui->lineEdit->text().toInt();
    info.name = ui->lineEdit_2->text();
    info.url = ui->lineEdit_3->text();
    info.square = ui->lineEdit_4->text().toDouble();
    info.position = ui->lineEdit_5->text();
    info.build_struct = ui->lineEdit_6->text();
    info.bim = ui->lineEdit_7->text();
    info.type = ui->lineEdit_8->text();
    info.height = ui->lineEdit_9->text();
    info.nature = ui->lineEdit_10->text();
    QDateTime current_date_time = QDateTime::currentDateTime();
    info.time = current_date_time.toString("yyyy/MM/dd");
    return info;
}

void AddItemDialog::on_pushButton_clicked()
{
    accept();
}

void AddItemDialog::on_pushButton_2_clicked()
{
    close();
}
