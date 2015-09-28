#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "browser.h"
#include "updateinfo.h"
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
    setupModel();
    setupConnection();
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::setupGui()
{
    splitter = new QSplitter(this);
    this->setCentralWidget(splitter);

    realTimeGroupBox = new QGroupBox(tr("realtime info"), this);
    infoGroupBox = new QGroupBox(tr("basic info"), this);
    splitter->addWidget(realTimeGroupBox);
    splitter->addWidget(infoGroupBox);

    mainWidget = new QTabWidget(this);
    QVBoxLayout* layout1 = new QVBoxLayout;
    layout1->addWidget(mainWidget);
    infoGroupBox->setLayout(layout1);

    searchEngineView = new QTableView(this);
    keyWordView = new QTableView(this);
    clickView = new QTableView(this);

    mainWidget->addTab(searchEngineView, tr("search engine"));
    mainWidget->addTab(keyWordView, tr("key words"));
    mainWidget->addTab(clickView, tr("clicks"));

    QGridLayout* layout2 = new QGridLayout;
    searchEngineLabel = new QLabel(tr("search engine"), this);
    searchEngineLabelValue = new QLabel(this);
    searchUrlLabel = new QLabel(tr("search engine url"), this);
    searchUrlLabelValue = new QLabel(this);
    keyWordLabel = new QLabel(tr("key word"), this);
    keyWordLabelValue = new QLabel(this);
    clickLinkLabel = new QLabel(tr("click link name"), this);
    clickLinkLabelValue = new QLabel(this);
    clickUrlLabel = new QLabel(tr("click link url"), this);
    clickUrlLabelValue = new QLabel(this);

    layout2->addWidget(searchEngineLabel, 0, 0, 1, 1);
    layout2->addWidget(searchEngineLabelValue, 0, 1, 1, 1);
    layout2->addWidget(searchUrlLabel, 1,0,1,1);
    layout2->addWidget(searchUrlLabelValue, 1,1,1,1);
    layout2->addWidget(keyWordLabel, 2,0,1,1);
    layout2->addWidget(keyWordLabelValue, 2,1,1,1);
    layout2->addWidget(clickLinkLabel, 3,0,1,1);
    layout2->addWidget(clickLinkLabelValue, 3,1,1,1);
    layout2->addWidget(clickUrlLabel, 4,0,1,1);
    layout2->addWidget(clickUrlLabelValue, 4,1,1,1);
    realTimeGroupBox->setLayout(layout2);

    toolBar = this->addToolBar(tr("main tool bar"));
    //add browser
    browser = new Browser(this);
    isRunning = false;
    this->resize(700, 400);
}

void MainWindow::setupAction()
{
    jobStateAction = new QAction(tr("startJob"), this);
    toolBar->addAction(jobStateAction);
}

void MainWindow::setupConnection()
{
    connect(jobStateAction, SIGNAL(triggered()), this, SLOT(onJobStateChange()));
    connect(browser, SIGNAL(updateClickInfo(UpdateInfo)), this, SLOT(onJobUpdate(UpdateInfo)));
}

void MainWindow::onJobUpdate(const UpdateInfo &updateInfo)
{
    this->searchEngineLabelValue->setText(updateInfo.engineName);
    this->searchUrlLabelValue->setText(updateInfo.engineUrl);
    this->keyWordLabelValue->setText(updateInfo.keyWord);
    this->clickLinkLabelValue->setText(updateInfo.clickName);
    this->clickUrlLabelValue->setText(updateInfo.clickUrl);

}

void MainWindow::onJobStateChange()
{
    if (isRunning == false)
    {
        jobStateAction->setText(tr("stopJob"));
        browser->show();
        browser->move(this->pos() + QPoint(200, 200));
        startSearchJob();
        isRunning = true;
    }else
    {
        jobStateAction->setText(tr("startJob"));
        browser->stopSearch();
        browser->hide();
        isRunning = false;
    }
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
void MainWindow::startSearchJob()
{
    QList<QString> engines = QList<QString>();
    QList<QString> urls = QList<QString>();
    QList<QString> keyWords = QList<QString>();
    for (int i = 0; i< 5; i++) {
        engines.append("百度");
        urls.append("http://www.baidu.com");
        keyWords.append("musper");
    }
    for (int i = 0; i < 5; i++) {
        engines.append("手机百度");
        urls.append("http://m.baidu.com");
        keyWords.append("musper");
    }
    browser->search(engines, urls, keyWords);
}
