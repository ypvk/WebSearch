#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}
class QTableView;
class QSqlTableModel;
class QSplitter;
class QGroupBox;
class QPushButton;
class QLabel;
class QToolBar;
class Browser;
class UpdateInfo;

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
    void setupConnection();
    void startSearchJob();
public slots:
    void onJobStateChange();
    void onJobUpdate(const UpdateInfo& updateInfo);
private:
    Ui::MainWindow *ui;
    QTabWidget* mainWidget;

    QTableView* searchEngineView;
    QTableView* keyWordView;
    QTableView* clickView;

    QSqlTableModel* searchEngineModel;
    QSqlTableModel* keyWordModel;
    QSqlTableModel* clickModel;

    QSplitter* splitter;
    QGroupBox* realTimeGroupBox;
    QGroupBox* infoGroupBox;
    QPushButton* enterButton;

    QLabel* searchEngineLabel;
    QLabel* searchEngineLabelValue;
    QLabel* searchUrlLabel;
    QLabel* searchUrlLabelValue;
    QLabel* keyWordLabel;
    QLabel* keyWordLabelValue;
    QLabel* clickLinkLabel;
    QLabel* clickLinkLabelValue;
    QLabel* clickUrlLabel;
    QLabel* clickUrlLabelValue;

    QAction* jobStateAction;
    QToolBar* toolBar;
    bool isRunning;

    Browser* browser;
};

#endif // MAINWINDOW_H
