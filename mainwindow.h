#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}
class QTableView;
class QSqlTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void setupGui();
    void setupAction();
    void setupModel();
private:
    Ui::MainWindow *ui;
    QTabWidget* mainWidget;

    QTableView* searchEngineView;
    QTableView* keyWordView;
    QTableView* clickView;

    QSqlTableModel* searchEngineModel;
    QSqlTableModel* keyWordModel;
    QSqlTableModel* clickModel;
};

#endif // MAINWINDOW_H
