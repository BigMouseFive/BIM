#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CaseSearch.h"
#include "DemoManager.h"
#include <QLayout>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // init widgets
    m_case_search = new CaseSearch();
    m_demo_manager = new DemoManager();
    auto hbox = new QHBoxLayout();
    hbox->addWidget(m_case_search);
    hbox->addWidget(m_demo_manager);
    m_demo_manager->hide();
    ui->centralWidget->setLayout(hbox);

    initToolBar();

    ui->statusBar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initToolBar(){
    ui->mainToolBar->addAction(QIcon(":/images/search"), QString::fromUtf8("方案查询"), [=](){
        m_demo_manager->hide();
        m_case_search->show();
    });
    ui->mainToolBar->addAction(QIcon(":/images/demo"), QString::fromUtf8("案例管理"), [=](){
        m_case_search->hide();
        m_demo_manager->show();
    });

    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addToolBar(Qt::LeftToolBarArea, ui->mainToolBar);
}
