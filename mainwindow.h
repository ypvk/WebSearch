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
class QLineEdit;
class Browser;
class UpdateInfo;
class ConfigDialog;
class QNetworkAccessManager;

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
    void startSearchJob(int clickNum, int threadNum);
    void runSearchJob(int threadNum);
    void queryProxys(const QString& url);
public slots:
    void onJobStateChange();
    void onJobUpdate(const UpdateInfo& updateInfo);
    void onConfigActionTrigger();
    void onJobFinished();
    void onAboutActiontrigger();
    void onJobFinishedById(int id);
private:
    Ui::MainWindow *ui;

    QTableView* clickView;

    QSqlTableModel* clickModel;

    QSplitter* splitter;
    QGroupBox* realTimeGroupBox;
    QGroupBox* infoGroupBox;
    QPushButton* enterButton;

    QLabel* searchEngineLabel;
    QLineEdit* searchEngineLabelValue;
    QLabel* searchUrlLabel;
    QLineEdit* searchUrlLabelValue;
    QLabel* keyWordLabel;
    QLineEdit* keyWordLabelValue;
    QLabel* clickLinkLabel;
    QLineEdit* clickLinkLabelValue;
    QLabel* clickUrlLabel;
    QLineEdit* clickUrlLabelValue;
    QLabel* ipLabel;
    QLineEdit* ipValue;

    QAction* startJobAction;
    QAction* stopJobAction;
    QAction* configAction;
    QAction* aboutAction;

    QToolBar* toolBar;
    bool isRunning;

    Browser* browser;
    ConfigDialog* configDialog;
    QNetworkAccessManager* networkManager;
    QList<QPair<QString, int> > proxys;
    QList<Browser*> browsers;

    int clickNum;
    int threadNum;
    QList<int> finishedIds;
};

#endif // MAINWINDOW_H
