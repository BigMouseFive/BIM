#include "DemoManager.h"
#include "ui_DemoManager.h"
#include <QApplication>
#include <QStandardItemModel>
#include <QFile>
#include <QDebug>
#include <DataManager.h>
#include <AddItemDialog.h>
DemoManager::DemoManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemoManager),
    m_model(nullptr)
{
    qApp->setStyleSheet(QFile(":/static/static/MacOS.qss").readAll());
    ui->setupUi(this);
    initTableViewHeader();
}

DemoManager::~DemoManager()
{
    delete ui;
}

void DemoManager::initTableViewHeader(){
    QStringList strHeader;
    strHeader << QString::fromLocal8Bit("案例编号")
              << QString::fromLocal8Bit("案例名称")
              << QString::fromLocal8Bit("存储时间")
              << QString::fromLocal8Bit("应用方案");

    if(nullptr == m_model)
    {
        m_model = new QStandardItemModel();
    }

    m_model->setHorizontalHeaderLabels(strHeader);
    m_model->setColumnCount(strHeader.size());
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    auto bims = DataManager::GetInstance()->GetBims();
    for (auto item = bims.begin(); item != bims.end(); ++item){
        BimInfo& data = item->second;
        QList<QStandardItem* > item_list;
        item_list.append(new QStandardItem(QString::number(data.num)));
        item_list.append(new QStandardItem(data.name));
        item_list.append(new QStandardItem(data.time));
        item_list.append(new QStandardItem(data.url));
        m_model->appendRow(item_list);
    }
}

// delete
void DemoManager::on_pushButton_2_clicked()
{
    if (ui->tableView->currentIndex() != QModelIndex()){
        auto index = m_model->index(ui->tableView->currentIndex().row(), 0, ui->tableView->currentIndex().parent());
        int num = m_model->data(index).toString().toInt();
        m_model->takeRow(ui->tableView->currentIndex().row());
        DataManager::GetInstance()->DelBimByNum(num);
    }
}

// add
void DemoManager::on_pushButton_clicked()
{
    AddItemDialog dialog;
    if (dialog.exec() == QDialog::Accepted){
        BimInfo bim_info = dialog.getBimInfo();
        if (bim_info.num != 0){
            DataManager::GetInstance()->AddBim(bim_info);
            QList<QStandardItem* > item_list;
            item_list.append(new QStandardItem(QString::number(bim_info.num)));
            item_list.append(new QStandardItem(bim_info.name));
            item_list.append(new QStandardItem(bim_info.time));
            item_list.append(new QStandardItem(bim_info.url));
            m_model->appendRow(item_list);
        }
    }
}
