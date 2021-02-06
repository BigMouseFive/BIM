#include "CaseSearch.h"
#include "ui_CaseSearch.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QStandardItemModel>
#include <QDebug>
#include <DataManager.h>
#include <QString>

std::map<QString, int> height_map;
CaseSearch::CaseSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CaseSearch),
    m_model(nullptr)
{
    ui->setupUi(this);
    initChineseData();
    initTableViewHeader();
    ui->tableView->setModel(m_model);

    height_map["低层"] = 0;
    height_map["多层"] = 1;
    height_map["高层"] = 2;
    height_map["超高层"] = 3;
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
    auto bim_info_map = DataManager::GetInstance()->GetBims();
    double max_square = DataManager::GetInstance()->getMaxSquare();
    double min_square = DataManager::GetInstance()->getMinSquare();
    for (auto iter = bim_info_map.begin(); iter != bim_info_map.end(); ++iter){
        BimInfo& bim_info = iter->second;
        // 建筑面积 square
        double sim_square = 1 - abs(ui->spin_square_size->value() - bim_info.square) / (max_square - min_square);

        // 建筑高度 height
        double sim_height = height_sim_map[ui->cb_height->currentIndex()][height_map[bim_info.height]];

        // 建筑结构 strcut
        QStringList struct_list = bim_info.build_struct.split(",");
        double count = 0;
        for (auto iter = m_struct_list.begin(); iter != m_struct_list.end(); ++iter){
            for (auto it = struct_list.begin(); it != struct_list.end(); ++it){
                if (*it == iter->text()){
                    count++;
                    break;
                }
            }
        }
        double sim_struct;
        if (struct_list.isEmpty())
            sim_struct = 0;
        else
            sim_struct = count / struct_list.size();

        // 建设地点 point
        QString point_text = "";
        double sim_point = 1 ? ui->cb_province->currentText() + ui->cb_city->currentText() + ui->cb_area->currentText() == point_text : 8;

        // BIM 应用阶段 bim
        QStringList bim_list = bim_info.bim.split(":");
        QStringList m_bim_list;
        if (ui->ck_design->checkState() == Qt::Checked) bim_list.append("设计");
        if (ui->ck_devops->checkState() == Qt::Checked) bim_list.append("运维");
        if (ui->ck_dismantle->checkState() == Qt::Checked) bim_list.append("拆除");
        if (ui->ck_reform->checkState() == Qt::Checked) bim_list.append("改造");
        if (ui->ck_survey->checkState() == Qt::Checked) bim_list.append("勘察");
        if (ui->ck_work->checkState() == Qt::Checked) bim_list.append("施工");
        count = 0;
        for (auto iter = m_bim_list.begin(); iter != m_bim_list.end(); ++iter){
            for (auto it = bim_list.begin(); it != bim_list.end(); ++it){
                if (*it == *iter){
                    count++;
                    break;
                }
            }
        }
        double sim_bim;
        if (bim_list.isEmpty())
            sim_bim = 0;
        else
            sim_bim = count / bim_list.size();

        // 项目类型 type
        QStringList type_list = bim_info.type.split(":");
        QStringList m_type_list;
        if (ui->ck_office->checkState() == Qt::Checked) m_type_list.append("办公");
        if (ui->ck_science->checkState() == Qt::Checked) m_type_list.append("科研");
        if (ui->ck_education->checkState() == Qt::Checked) m_type_list.append("教育");
        if (ui->ck_dorm->checkState() == Qt::Checked) m_type_list.append("宿舍");
        if (ui->ck_entertainment->checkState() == Qt::Checked) m_type_list.append("文娱");
        if (ui->ck_traffic->checkState() == Qt::Checked) m_type_list.append("交通");
        if (ui->ck_communication->checkState() == Qt::Checked) m_type_list.append("通信");
        if (ui->ck_medical->checkState() == Qt::Checked) m_type_list.append("医疗");
        if (ui->ck_finance->checkState() == Qt::Checked) m_type_list.append("金融");
        if (ui->ck_gardens->checkState() == Qt::Checked) m_type_list.append("园林");
        if (ui->ck_agriculture->checkState() == Qt::Checked) m_type_list.append("农业");
        if (ui->ck_civil->checkState() == Qt::Checked) m_type_list.append("民政");
        if (ui->ck_sport->checkState() == Qt::Checked) m_type_list.append("体育");
        if (ui->ck_justice->checkState() == Qt::Checked) m_type_list.append("司法");
        if (ui->ck_religion->checkState() == Qt::Checked) m_type_list.append("宗教");
        if (ui->ck_transfer->checkState() == Qt::Checked) m_type_list.append("物流");
        if (ui->ck_industry->checkState() == Qt::Checked) m_type_list.append("工业");
        if (ui->ck_anniversary->checkState() == Qt::Checked) m_type_list.append("纪念");
        if (ui->ck_home->checkState() == Qt::Checked) m_type_list.append("住宅");
        if (ui->ck_business->checkState() == Qt::Checked) m_type_list.append("商业");
        count = 0;
        for (auto iter = m_type_list.begin(); iter != m_type_list.end(); ++iter){
            for (auto it = type_list.begin(); it != type_list.end(); ++it){
                if (*it == *iter){
                    count++;
                    break;
                }
            }
        }
        double sim_type;
        if (type_list.isEmpty())
            sim_type = 0;
        else
            sim_type = count / type_list.size();

        // 建设性质 nature
        QStringList nature_list = bim_info.nature.split(":");
        QStringList m_nature_list;
        if (ui->ck_resume->checkState() == Qt::Checked) m_nature_list.append("恢复");
        if (ui->ck_move->checkState() == Qt::Checked) m_nature_list.append("迁建");
        if (ui->ck_new->checkState() == Qt::Checked) m_nature_list.append("新建");
        if (ui->ck_expand->checkState() == Qt::Checked) m_nature_list.append("扩建");
        if (ui->ck_update->checkState() == Qt::Checked) m_nature_list.append("改建");
        count = 0;
        for (auto iter = m_nature_list.begin(); iter != m_nature_list.end(); ++iter){
            for (auto it = nature_list.begin(); it != nature_list.end(); ++it){
                if (*it == *iter){
                    count++;
                    break;
                }
            }
        }
        double nature_type;
        if (nature_list.isEmpty())
            nature_type = 0;
        else
            nature_type = count / nature_list.size();

        // 关键词 key

    }
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
    m_struct_list.emplace_back(line_edit);
    connect(btn_del, &QPushButton::clicked, [=](){
        ui->vbox_struct->removeWidget(widget);
        widget->deleteLater();
        for (auto iter = m_struct_list.begin(); iter != m_struct_list.end(); ++iter){
            if (*iter == line_edit){
                m_struct_list.erase(iter);
                break;
            }
        }
    });
}

void CaseSearch::on_btn_add_key_clicked()
{
    auto widget = new QWidget(this);
    auto line_edit = new QLineEdit();
    auto spin_box = new QDoubleSpinBox();
    spin_box->setPrefix(QString::fromUtf8("权重"));
    auto btn_del = new QPushButton(QIcon(":/images/images/del.png"), QString::fromUtf8("删除"));
    auto hbox = new QHBoxLayout();
    hbox->addWidget(line_edit);
    hbox->addWidget(spin_box);
    hbox->addWidget(btn_del);
    hbox->setMargin(0);
    hbox->setSpacing(8);
    widget->setLayout(hbox);
    ui->vbox_key->addWidget(widget);
    m_key_list.emplace_back(line_edit);
    connect(btn_del, &QPushButton::clicked, [=](){
        ui->vbox_key->removeWidget(widget);
        widget->deleteLater();
        for (auto iter = m_key_list.begin(); iter != m_key_list.end(); ++iter){
            if (*iter == line_edit){
                m_key_list.erase(iter);
                break;
            }
        }
    });
}
