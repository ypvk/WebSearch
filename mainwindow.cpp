#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "browser.h"
#include "updateinfo.h"
#include "dbutil.h"
#include "jobparamdialog.h"
#include "configdialog.h"
#include "engineinfo.h"
#include "aboutdialog.h"
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtCore/QString>
#include <QtSql>
#include <QtGui>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

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

    realTimeGroupBox = new QGroupBox(tr("实时信息"), this);
    infoGroupBox = new QGroupBox(tr("点击信息"), this);
    splitter->addWidget(realTimeGroupBox);
    splitter->addWidget(infoGroupBox);
    splitter->setStretchFactor(1, 70);


    clickView = new QTableView(this);
    clickView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    clickView->horizontalHeader()->setStretchLastSection(true);

    QVBoxLayout* layout1 = new QVBoxLayout;
    layout1->addWidget(clickView);
    infoGroupBox->setLayout(layout1);

    deleteSelectedButton = new QPushButton(tr("删除选中"), this);
    deleteAllButton = new QPushButton(tr("清空"), this);
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(deleteSelectedButton);
    buttonLayout->addWidget(deleteAllButton);
    layout1->addLayout(buttonLayout);


    QGridLayout* layout2 = new QGridLayout;
    searchEngineLabel = new QLabel(tr("搜索引擎"), this);
    searchEngineLabelValue = new QLineEdit(this);
    searchUrlLabel = new QLabel(tr("地址"), this);
    searchUrlLabelValue = new QLineEdit(this);
    keyWordLabel = new QLabel(tr("关键词"), this);
    keyWordLabelValue = new QLineEdit(this);
    clickLinkLabel = new QLabel(tr("链接名"), this);
    clickLinkLabelValue = new QLineEdit(this);
    clickUrlLabel = new QLabel(tr("链接地址"), this);
    clickUrlLabelValue = new QLineEdit(this);
    ipLabel = new QLabel(tr("ip"), this);
    ipValue = new QLineEdit(this);

    layout2->setColumnStretch(0, 1);
    layout2->setColumnStretch(1, 2);
//    layout2->setSizeConstraint(QLayout::SetFixedSize);

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
    layout2->addWidget(ipLabel, 5,0, 1,1);
    layout2->addWidget(ipValue, 5,1,1,1);
    realTimeGroupBox->setLayout(layout2);
//    realTimeGroupBox->setSizePolicy(QSizePolicy::Fixed ,QSizePolicy::Expanding);

    toolBar = this->addToolBar(tr("main tool bar"));
    //add browser
//    browser = new Browser(this);
    configDialog = new ConfigDialog(this);
    isRunning = false;
    this->resize(800, 400);
    this->setWindowTitle("Websearch");
    networkManager = new QNetworkAccessManager(this);
}

void MainWindow::setupAction()
{
    startJobAction = new QAction(tr("开始任务"), this);
    stopJobAction = new QAction(tr("停止任务"), this);
    configAction = new QAction(tr("配置"), this);
    aboutAction = new QAction(tr("关于"), this);

    stopJobAction->setEnabled(false);

    toolBar->addAction(startJobAction);
    toolBar->addAction(stopJobAction);
    toolBar->addAction(configAction);
    toolBar->addAction(aboutAction);
}

void MainWindow::setupConnection()
{
    connect(startJobAction, SIGNAL(triggered()), this, SLOT(onJobStateChange()));
    connect(stopJobAction, SIGNAL(triggered()), this, SLOT(onJobStateChange()));
//    connect(browser, SIGNAL(updateClickInfo(UpdateInfo)), this, SLOT(onJobUpdate(UpdateInfo)));
//    connect(browser, SIGNAL(jobFinished()), this, SLOT(onJobFinished()));
    connect(configAction, SIGNAL(triggered()), this, SLOT(onConfigActionTrigger()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(onAboutActiontrigger()));
    connect(deleteAllButton, SIGNAL(clicked()), this, SLOT(onDeleteAllButtonClicked()));
    connect(deleteSelectedButton, SIGNAL(clicked()), this, SLOT(onDeleteSelectedButtonClicked()));
}

void MainWindow::onConfigActionTrigger()
{
    configDialog->exec();
}
void MainWindow::onAboutActiontrigger()
{
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::onJobUpdate(const UpdateInfo &updateInfo)
{
    this->searchEngineLabelValue->setText(updateInfo.engineName);
    this->searchUrlLabelValue->setText(updateInfo.engineUrl);
    this->keyWordLabelValue->setText(updateInfo.keyWord.first + "&" + updateInfo.keyWord.second);
    this->clickLinkLabelValue->setText(updateInfo.clickName);
    this->clickUrlLabelValue->setText(updateInfo.clickUrl);
    this->ipValue->setText(updateInfo.ip);
    bool result;
    if(!updateInfo.clickName.isEmpty()) result = DBUtil::incWorkClick(updateInfo.keyWord.first,
                                                                      updateInfo.keyWord.second,
                                                                      updateInfo.engineUrl);
    if (!result)
    {
        qDebug() << "error update";
    }
    else
    {
        clickModel->select();
    }
}

void MainWindow::onJobStateChange()
{
    if (isRunning == false)
    {
        JobParamDialog dialog;
        if (dialog.exec() == QDialog::Accepted) {
            startJobAction->setEnabled(false);
//            browser->show();
//            browser->move(this->pos() + QPoint(200, 200));
            startSearchJob(dialog.getClickNum(), dialog.getThreadNum());
            isRunning = true;
            stopJobAction->setEnabled(true);
        }
        else return;
    }else
    {
       this->clickNum = 1;
       this->onJobFinished();
    }
}
void MainWindow::onJobFinished()
{
    stopJobAction->setEnabled(false);
//    browser->stopSearch();
//    browser->hide();
    isRunning = false;
    startJobAction->setEnabled(true);
    //delete browser
    foreach(Browser* browser, browsers)  {
        delete browser;
    }
    browsers.clear();
    finishedIds.clear();
    qDebug() << "finished query: " << this->clickNum;
    this->clickNum = this->clickNum - 1;
    if (this->clickNum != 0) {
        this->sleep(120000);
        runSearchJob(threadNum);
    }
}
void MainWindow::onJobFinishedById(int id)
{
    Browser* browser = browsers.at(id);
//    browsers.removeAt(id);
    browser->close();
    finishedIds.append(id);
    if (finishedIds.count() == browsers.count())
    {
        finishedIds.clear();
        this->onJobFinished();
    }
//    delete browser;
//    if (browsers.isEmpty())
//        this->onJobFinished();
}

void MainWindow::setupModel()
{
    QSqlDatabase db = DBUtil::getDB();
    clickModel = new QSqlTableModel(this, db);
    clickModel->setTable(DBUtil::CLICK_TABLE_NAME);
    clickModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    clickModel->select();
    clickModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    clickModel->setHeaderData(1, Qt::Horizontal, QObject::tr("ENGINE URL"));
    clickModel->setHeaderData(2, Qt::Horizontal, QObject::tr("MAIN WORD"));
    clickModel->setHeaderData(3, Qt::Horizontal, QObject::tr("ASSIST WORD"));
    clickModel->setHeaderData(4, Qt::Horizontal, QObject::tr("CLICK TIMES"));
    clickView->setModel(clickModel);

}
void MainWindow::startSearchJob(int clickNum, int threadNum)
{
    this->clickNum = clickNum;
    this->threadNum = threadNum;
    runSearchJob(threadNum);
}
void MainWindow::runSearchJob(int threadNum)
{
    //TODO change

    QList<QPair<QString, QString> > keyWords = DBUtil::getKeyWords();
    QList<EngineInfo> engineInfos = DBUtil::getEngineInfos();
    if (configDialog->isUseProxyApi())
    {
        queryProxys(configDialog->getProxyApiValue());
    }
    else
    {
        proxys = DBUtil::getProxys();
    }
    if (keyWords.isEmpty() || engineInfos.isEmpty())
    {
        qDebug() << "empty job";
        this->onJobFinished();
        return;
    }
    int deta = proxys.count() / threadNum;
    if (deta == 0) deta = 1;

    for(int i = 0; i < threadNum; i++)
    {
        QList<QPair<QString, int> > tmpProxys;
        for (int j = 0; j < deta; j++)
        {
            if(!proxys.isEmpty()) {
                tmpProxys << proxys.first();
                proxys.pop_front();
            }
        }
        //build clickInfo
        QList<ClickInfo> clickInfos;
        for (int j = 0; j < engineInfos.size(); j++)
        {
            clickInfos << ClickInfo(engineInfos.at(j), keyWords, tmpProxys, 1);
        }
        Browser* browser = new Browser(NULL, i);
        connect(browser, SIGNAL(updateClickInfo(UpdateInfo)), this, SLOT(onJobUpdate(UpdateInfo)));
        connect(browser, SIGNAL(jobFinished(int)), this, SLOT(onJobFinishedById(int)));
        browser->show();
        browser->move(this->pos() + QPoint(200, 200));
        browsers.append(browser);
        browser->search(clickInfos);
    }
}

void MainWindow::queryProxys(const QString& url)
{
    proxys.clear();
    QNetworkRequest request;
    QUrl _url(url);
    request.setUrl(_url);
    QNetworkReply* reply = networkManager->get(request);
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(5000);
    loop.exec();
    timer.stop();
    if (reply->isFinished())
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QString value = reply->readAll();
            QStringList values = value.split("\n");
            for(int i = 0; i < values.count(); i++)
            {
                QString proxyUrl = values.at(i).trimmed();
                if (proxyUrl.contains(":") && !proxyUrl.isEmpty())
                {
                    QStringList urlAndPort = proxyUrl.split(":");
                    proxys << QPair<QString, int>(urlAndPort.at(0), urlAndPort.at(1).toInt());
                }

            }

        }
        else
        {
            qDebug() << "error data";
        }
    }
    else
    {
        networkManager->setNetworkAccessible(QNetworkAccessManager::NotAccessible);
        qDebug() << "timeout";
    }
}

void MainWindow::sleep(long time)
{
        QEventLoop loop;
        QTimer timer;
        timer.setSingleShot(true);
        QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.start(time);
        loop.exec();
        timer.stop();
}
void MainWindow::onDeleteSelectedButtonClicked()
{
    QItemSelectionModel *selections = this->clickView->selectionModel();
    QModelIndexList indexes = selections->selectedIndexes();
    if (indexes.count() == 0) {
        return;
    }
    QVariantList ids;
    foreach(QModelIndex index, indexes)
    {
        if(index.column() == 0) {
            ids << index.data();
        }
    }
    bool result = DBUtil::deleteClicks(ids);
    if (! result) {
        QMessageBox::information(this, "error", "error delete engine");
    }
    else
        clickModel->select();
}

void MainWindow::onDeleteAllButtonClicked()
{
    DBUtil::clearTable(DBUtil::CLICK_TABLE_NAME);
    clickModel->select();
}
