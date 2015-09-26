#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbutil.h"
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtCore/QString>
#include <QtSql>
#include <QtGui>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGui();
    setupAction();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupGui()
{
    mainWidget = new QTabWidget(this);
    this->setCentralWidget(mainWidget);

    searchEngineView = new QTableView(this);
    keyWordView = new QTableView(this);
    clickView = new QTableView(this);

    mainWidget->addTab(searchEngineView, tr("search engine"));
    mainWidget->addTab(keyWordView, tr("key words"));
    mainWidget->addTab(clickView, tr("clicks"));
}

void MainWindow::setupAction()
{

}

void MainWindow::setupModel()
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

    clickModel = new QSqlTableModel(this, db);
    clickModel->setTable(DBUtil::CLICK_TABLE_NAME);
    clickModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    clickModel->select();
    clickModel->setHeaderData(0, Qt::Horizontal, QObject::tr("search engine id"));
    clickModel->setHeaderData(1, Qt::Horizontal, QObject::tr("key word id"));
    clickModel->setHeaderData(2, Qt::Horizontal, QObject::tr("click times"));
    clickView->setModel(clickModel);

}
