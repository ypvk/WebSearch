#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

class QTableView;
class QTableModel;
class QSqlTableModel;
class QGroupBox;
class QTabWidget;
class QPushButton;

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);
private:
    void setupGui();
    void setupModel();
    void setupConnection();
    QString getFileName();
    void showMessage(const QString& msg);
    void showSuccessMessage(const QString& msg);
protected slots:
    void onLoadForSearchButtonClicked();
    void onLoadForKeyWordButtonClicked();
    void onLoadForProxyButtonClicked();
    void onRemoveSelectedForSearchButtonClicked();
    void onRemoveSelectedForKeyWordButtonClicked();
    void onRemoveSelectedForProxyButtonClicked();
    void onSubmitChangeForSearchButtonClicked();
    void onSubmitChangeForKeyWordButtonClicked();
    void onSubmitChangeForProxyButtonClicked();

private:

    QTabWidget* mainWidget;

    QTableView* searchEngineView;
    QTableView* keyWordView;
    QTableView* proxyView;

    QSqlTableModel* searchEngineModel;
    QSqlTableModel* keyWordModel;
    QSqlTableModel* proxyModel;

    QWidget* searchEngineWidget;
    QWidget* keyWordWidget;
    QWidget* proxyWidget;

    QGroupBox* searchEngineGroupBox;
    QGroupBox* searchEngineOperationBox;
    QGroupBox* keyWordGroupBox;
    QGroupBox* keyWordOperationBox;
    QGroupBox* proxyGroupBox;
    QGroupBox* proxyOperationBox;


    QPushButton* loadForSearchButton;
    QPushButton* loadForKeyWordButton;
    QPushButton* loadForProxyButton;
    QPushButton* removeSelectedForSearchButton;
    QPushButton* removeSelectedForKeyWordButton;
    QPushButton* removeSelectedForProxyButton;
    QPushButton* submitChangeForSearchButton;
    QPushButton* submitChangeForKeyWordButton;
    QPushButton* submitChangeForProxyButton;

};

#endif // CONFIGDIALOG_H
