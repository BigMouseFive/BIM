#include "DemoManager.h"
#include "ui_DemoManager.h"
#include <QApplication>
#include <QStandardItemModel>
#include <QFile>
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
}
