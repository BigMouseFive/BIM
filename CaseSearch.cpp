#include "CaseSearch.h"
#include "ui_CaseSearch.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QStandardItemModel>
#include <QDebug>
CaseSearch::CaseSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CaseSearch),
    m_model(nullptr)
{
    ui->setupUi(this);
    initChineseData();
    initTableViewHeader();
    ui->tableView->setModel(m_model);
}

CaseSearch::~CaseSearch()
{
    delete ui;
}

void CaseSearch::initChineseData(){
    QFile loadFile(":/static/static/chinese.json");
    loadFile.open(QIODevice::ReadOnly);
    QByteArray allData = loadFile.readAll();
    loadFile.close();
    QJsonParseError json_error;
    auto json_doc = QJsonDocument::fromJson(allData, &json_error);
    chinese_data = json_doc.array();

    for (auto& item: chinese_data){
        ui->cb_province->addItem(item.toObject()["name"].toString());
    }
}

void CaseSearch::initTableViewHeader(){
    QStringList strHeader;
    strHeader << QString::fromLocal8Bit("案例编号")
              << QString::fromLocal8Bit("案例名称")
              << QString::fromLocal8Bit("相似度")
              << QString::fromLocal8Bit("应用方案");

    if(nullptr == m_model)
        m_model = new QStandardItemModel();

    m_model->setHorizontalHeaderLabels(strHeader);
    m_model->setColumnCount(strHeader.size());
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}
void CaseSearch::on_btn_show_close_param_clicked()
{
    ui->groupBox_param->setHidden(show_flag);
    ui->groupBox_weight->setHidden(show_flag);
    show_flag = !show_flag;
    if (show_flag){
        ui->btn_show_close_param->setIcon(QIcon(":/images/images/close.png"));
        ui->btn_show_close_param->setText(QString::fromLocal8Bit("收起搜索参数"));
    }
    else{
        ui->btn_show_close_param->setIcon(QIcon(":/images/images/show.png"));
        ui->btn_show_close_param->setText(QString::fromLocal8Bit("展开搜索参数"));
    }
}

double height_sim_map[4][4] = {1, 1/9.0, 0, 0,
                               1/9.0, 1, 1/7.0, 0,
                               0, 1/7.0, 1, 1/9.0,
                               0, 0, 1/9.0, 1};

// 计算二进制中1的个数
int BitCount2(unsigned int n)
{
    unsigned int c =0 ;
    for (c =0; n; ++c)
    {
        n &= (n -1) ; // 清除最低位的1
    }
    return c ;
}
void CaseSearch::on_btn_search_clicked()
{
    m_model->clear();
    initTableViewHeader();

    // 建筑面积 square
    double item_square = 4; // todo
    double max_square = 100; // todo
    double min_square = 0; // todo
    double sim_square = 1 - abs(ui->spin_square_size->value() - item_square) / (max_square - min_square);

    // 建筑高度 height
    int item_height_index = 0;  // 0低层 1多层 2高层 3超高层
    double sim_height = height_sim_map[ui->cb_height->currentIndex()][item_height_index];

    // 建筑结构 strcut

    // 建设地点 point
    QString point_text = "";
    sim_point = 1 ? ui->cb_province->currentText() + ui->cb_city->currentText() + ui->cb_area->currentText() == point_text : 0

    // BIM 应用阶段 bim


    // 项目类型 type

    // 建设性质 nature

}

void CaseSearch::on_cb_province_currentIndexChanged(const QString &arg1)
{
    ui->cb_city->clear();
    for (auto& item: chinese_data){
        if (arg1 == item.toObject()["name"].toString()){
            for (auto& item1: item.toObject()["city"].toArray()){
                ui->cb_city->addItem(item1.toObject()["name"].toString());
            }
            return;
        }
    }
}

void CaseSearch::on_cb_city_currentIndexChanged(const QString &arg1)
{
    ui->cb_area->clear();
    auto provience_name = ui->cb_province->currentText();
    for (auto& item: chinese_data){
        if (provience_name == item.toObject()["name"].toString()){
            for (auto& item1: item.toObject()["city"].toArray()){
                if (arg1 == item1.toObject()["name"].toString()){
                    for (auto& item2: item1.toObject()["area"].toArray()){
                        ui->cb_area->addItem(item2.toString());
                    }
                    return;
                }
            }
        }
    }
}

void CaseSearch::on_btn_add_struct_clicked()
{
    auto widget = new QWidget(this);
    auto line_edit = new QLineEdit();
    auto btn_del = new QPushButton(QIcon(":/images/images/del.png"), QString::fromLocal8Bit("删除"));
    auto hbox = new QHBoxLayout();
    hbox->addWidget(line_edit);
    hbox->addWidget(btn_del);
    hbox->setMargin(0);
    hbox->setSpacing(8);
    widget->setLayout(hbox);
    ui->vbox_struct->addWidget(widget);
    connect(btn_del, &QPushButton::clicked, [=](){
        ui->vbox_struct->removeWidget(widget);
        widget->deleteLater();
    });
}

void CaseSearch::on_btn_add_key_clicked()
{
    auto widget = new QWidget(this);
    auto line_edit = new QLineEdit();
    auto spin_box = new QSpinBox();
    spin_box->setPrefix(QString::fromUtf8("权重 "));
    auto btn_del = new QPushButton(QIcon(":/images/images/del.png"), QString::fromUtf8("删除"));
    auto hbox = new QHBoxLayout();
    hbox->addWidget(line_edit);
    hbox->addWidget(spin_box);
    hbox->addWidget(btn_del);
    hbox->setMargin(0);
    hbox->setSpacing(8);
    widget->setLayout(hbox);
    ui->vbox_key->addWidget(widget);
    connect(btn_del, &QPushButton::clicked, [=](){
        ui->vbox_key->removeWidget(widget);
        widget->deleteLater();
    });
}
