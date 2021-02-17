#ifndef CASESEARCH_H
#define CASESEARCH_H

#include <QWidget>
#include <QJsonArray>
namespace Ui {
class CaseSearch;
}
class QStandardItemModel;
class QLineEdit;
class QDoubleSpinBox;
class CaseSearch : public QWidget
{
    Q_OBJECT

public:
    CaseSearch(QWidget *parent = 0);
    ~CaseSearch();

private slots:
    void on_btn_show_close_param_clicked();

    void on_btn_search_clicked();

    void on_cb_province_currentIndexChanged(const QString &arg1);

    void on_cb_city_currentIndexChanged(const QString &arg1);

    void on_btn_add_struct_clicked();

    void on_btn_add_key_clicked();

private:
    void initChineseData();
    void initTableViewHeader();

    QStandardItemModel* m_model;
    QJsonArray chinese_data;
    bool show_flag = true;
    Ui::CaseSearch *ui;

    std::vector<QLineEdit*> m_struct_list;
    std::vector<QLineEdit*> m_key_list;
    std::vector<QDoubleSpinBox*> m_key_weight_list;
};

#endif // CASESEARCH_H
