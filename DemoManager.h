#ifndef DEMOMANAGER_H
#define DEMOMANAGER_H

#include <QWidget>

namespace Ui {
class DemoManager;
}
class QStandardItemModel;
class DemoManager : public QWidget
{
    Q_OBJECT

public:
    explicit DemoManager(QWidget *parent = 0);
    ~DemoManager();

protected:
    void initTableViewHeader();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:

    QStandardItemModel* m_model;
    Ui::DemoManager *ui;
};

#endif // DEMOMANAGER_H
