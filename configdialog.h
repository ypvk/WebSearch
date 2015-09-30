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
    void onRemoveSelectedForSearchButtonClicked();
    void onRemoveSelectedForKeyWordButtonClicked();
    void onSubmitChangeForSearchButtonClicked();
    void onSubmitChangeForKeyWordButtonClicked();

private:

    QTabWidget* mainWidget;

    QTableView* searchEngineView;
    QTableView* keyWordView;

    QSqlTableModel* searchEngineModel;
    QSqlTableModel* keyWordModel;

    QWidget* searchEngineWidget;
    QWidget* keyWordWidget;

    QGroupBox* searchEngineGroupBox;
    QGroupBox* searchEngineOperationBox;
    QGroupBox* keyWordGroupBox;
    QGroupBox* keyWordOperationBox;

    QPushButton* loadForSearchButton;
    QPushButton* loadForKeyWordButton;
    QPushButton* removeSelectedForSearchButton;
    QPushButton* removeSelectedForKeyWordButton;
    QPushButton* submitChangeForSearchButton;
    QPushButton* submitChangeForKeyWordButton;

};

#endif // CONFIGDIALOG_H
