#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class CaseSearch;
class DemoManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initToolBar();

    CaseSearch* m_case_search;
    DemoManager* m_demo_manager;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
