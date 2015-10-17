#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QSettings>

class QTableView;
class QTableModel;
class QSqlTableModel;
class QGroupBox;
class QTabWidget;
class QPushButton;
class QRadioButton;
class QLineEdit;
class QSettings;

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);
    bool isUseProxyApi();
    QString getProxyApiValue();
private:
    void setupGui();
    void setupModel();
    void setupConnection();
    QString getFileName();
    void showMessage(const QString& msg);
    void showSuccessMessage(const QString& msg);
    void loadSettings();
    void saveSettings();
protected:
    void closeEvent(QCloseEvent *);
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
    void onRaidoButtonToogled(bool state);
    void onClearSearchButtonClicked();
    void onClearKeyButtonClicked();
    void onClearProxyButton();

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
    QPushButton* clearSearchButton;
    QPushButton* clearKeyButton;
    QPushButton* clearProxyButton;

    QRadioButton* radioButton;
    QLineEdit* proxyApiValue;
    QSettings settings;

};

#endif // CONFIGDIALOG_H
