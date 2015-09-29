#include "configdialog.h"
#include "dbutil.h"
#include <QtGui>
#include <QtSql>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent)
{
    setupGui();
    setupModel();
    setupConnection();
}

void ConfigDialog::setupGui()
{
    mainWidget = new QTabWidget(this);
    QVBoxLayout* layout1 = new QVBoxLayout;
    layout1->addWidget(mainWidget);

    searchEngineView = new QTableView(this);
    keyWordView = new QTableView(this);

    searchEngineWidget = new QWidget(this);
    keyWordWidget = new QWidget(this);

    loadForSearchButton = new QPushButton(tr("load"), this);
    loadForKeyWordButton = new QPushButton(tr("load"), this);
    removeSelectedForSearchButton = new QPushButton(tr("remove selected"), this);
    removeSelectedForKeyWordButton = new QPushButton(tr("remove selected"), this);;
    submitChangeForSearchButton = new QPushButton(tr("submit"), this);
    submitChangeForKeyWordButton = new QPushButton(tr("submit"), this);


    searchEngineGroupBox = new QGroupBox(tr("search engine"), this);
    searchEngineOperationBox = new QGroupBox(tr("operation"), this);

    QVBoxLayout* searchEngineWidgetLayout = new QVBoxLayout;
    searchEngineWidgetLayout->addWidget(searchEngineGroupBox);
    searchEngineWidgetLayout->addWidget(searchEngineOperationBox);
    searchEngineWidget->setLayout(searchEngineWidgetLayout);

    QHBoxLayout* seGroupBoxLayout = new QHBoxLayout;
    seGroupBoxLayout->addWidget(searchEngineView);
    searchEngineGroupBox->setLayout(seGroupBoxLayout);
    QHBoxLayout* seOperationBoxLayout = new QHBoxLayout;
    seOperationBoxLayout->addWidget(loadForSearchButton);
    seOperationBoxLayout->addWidget(removeSelectedForSearchButton);
    seOperationBoxLayout->addWidget(submitChangeForSearchButton);
    searchEngineOperationBox->setLayout(seOperationBoxLayout);


    keyWordGroupBox = new QGroupBox(tr("key word"), this);
    keyWordOperationBox = new QGroupBox(tr("operation"), this);

    QVBoxLayout* keyWordWidgetLayout = new QVBoxLayout;
    keyWordWidgetLayout->addWidget(keyWordGroupBox);
    keyWordWidgetLayout->addWidget(keyWordOperationBox);
    keyWordWidget->setLayout(keyWordWidgetLayout);

    QHBoxLayout* kwGroupBoxLayout = new QHBoxLayout;
    kwGroupBoxLayout->addWidget(keyWordView);
    keyWordGroupBox->setLayout(kwGroupBoxLayout);

    QHBoxLayout* kwOperationBoxLayout = new QHBoxLayout;
    kwOperationBoxLayout->addWidget(loadForKeyWordButton);
    kwOperationBoxLayout->addWidget(removeSelectedForKeyWordButton);
    kwOperationBoxLayout->addWidget(submitChangeForKeyWordButton);
    keyWordOperationBox->setLayout(kwOperationBoxLayout);


    mainWidget->addTab(searchEngineWidget, tr("search engine config"));
    mainWidget->addTab(keyWordWidget, tr("key word config"));

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(mainWidget);
    this->setLayout(mainLayout);
}

void ConfigDialog::setupModel()
{
    QSqlDatabase db = DBUtil::getDB();
    searchEngineModel = new QSqlTableModel(this, db);
    searchEngineModel->setTable(DBUtil::SEARCH_ENGINE_NAME);
    searchEngineModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    searchEngineModel->select();
    searchEngineModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    searchEngineModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    searchEngineModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Url"));
    searchEngineView->setModel(searchEngineModel);

    keyWordModel = new QSqlTableModel(this, db);
    keyWordModel->setTable(DBUtil::KEY_WORD_NAME);
    keyWordModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    keyWordModel->select();
    keyWordModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    keyWordModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    keyWordView->setModel(keyWordModel);
}

void ConfigDialog::setupConnection()
{

}
