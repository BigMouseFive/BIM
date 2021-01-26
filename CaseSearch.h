#ifndef CASESEARCH_H
#define CASESEARCH_H

#include <QWidget>
#include <QJsonArray>
namespace Ui {
class CaseSearch;
}
class QStandardItemModel;
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
};

#endif // CASESEARCH_H
