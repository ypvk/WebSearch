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
    void startSearchJob(int clickNum);
public slots:
    void onJobStateChange();
    void onJobUpdate(const UpdateInfo& updateInfo);
    void onConfigActionTrigger();
    void onJobFinished();
    void onAboutActiontrigger();
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
};

#endif // MAINWINDOW_H
