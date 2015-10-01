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
    searchEngineView->setSelectionBehavior(QAbstractItemView::SelectRows);
    searchEngineView->horizontalHeader()->setStretchLastSection(true);

    keyWordView = new QTableView(this);
    keyWordView->setSelectionBehavior(QAbstractItemView::SelectRows);
    keyWordView->horizontalHeader()->setStretchLastSection(true);

    proxyView = new QTableView(this);
    proxyView->setSelectionBehavior(QAbstractItemView::SelectRows);
    proxyView->horizontalHeader()->setStretchLastSection(true);

    searchEngineWidget = new QWidget(this);
    keyWordWidget = new QWidget(this);
    proxyWidget = new QWidget(this);

    loadForSearchButton = new QPushButton(tr("load"), this);
    loadForKeyWordButton = new QPushButton(tr("load"), this);
    loadForProxyButton = new QPushButton(tr("load"), this);
    removeSelectedForSearchButton = new QPushButton(tr("remove selected"), this);
    removeSelectedForKeyWordButton = new QPushButton(tr("remove selected"), this);
    removeSelectedForProxyButton = new QPushButton(tr("remove selected"), this);
    submitChangeForSearchButton = new QPushButton(tr("submit"), this);
    submitChangeForKeyWordButton = new QPushButton(tr("submit"), this);
    submitChangeForProxyButton = new QPushButton(tr("submit"), this);


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
    //proxy
    proxyGroupBox = new QGroupBox(tr("proxy"), this);
    proxyOperationBox = new QGroupBox(tr("operation"), this);

    QVBoxLayout* proxyWidgetLayout = new QVBoxLayout;
    proxyWidgetLayout->addWidget(proxyGroupBox);
    proxyWidgetLayout->addWidget(proxyOperationBox);
    proxyWidget->setLayout(proxyWidgetLayout);

    QHBoxLayout* pxGroupBoxLayout = new QHBoxLayout;
    pxGroupBoxLayout->addWidget(proxyView);
    proxyGroupBox->setLayout(pxGroupBoxLayout);

    QHBoxLayout* pxOperationBoxLayout = new QHBoxLayout;
    pxOperationBoxLayout->addWidget(loadForProxyButton);
    pxOperationBoxLayout->addWidget(removeSelectedForProxyButton);
    pxOperationBoxLayout->addWidget(submitChangeForProxyButton);
    proxyOperationBox->setLayout(pxOperationBoxLayout);


    mainWidget->addTab(searchEngineWidget, tr("search engine config"));
    mainWidget->addTab(keyWordWidget, tr("key word config"));
    mainWidget->addTab(proxyWidget, tr("proxy widget"));

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(mainWidget);
    this->setLayout(mainLayout);
    this->resize(400,500);
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

    proxyModel = new QSqlTableModel(this, db);
    proxyModel->setTable(DBUtil::PROXY_NAME);
    proxyModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    proxyModel->select();
    proxyModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    proxyModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Url"));
    proxyModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Port"));
    proxyView->setModel(proxyModel);
}

void ConfigDialog::setupConnection()
{
    connect(loadForSearchButton, SIGNAL(clicked()), this, SLOT(onLoadForSearchButtonClicked()));
    connect(loadForKeyWordButton, SIGNAL(clicked()), this, SLOT(onLoadForKeyWordButtonClicked()));
    connect(loadForProxyButton, SIGNAL(clicked()), this, SLOT(onLoadForProxyButtonClicked()));
    connect(removeSelectedForSearchButton, SIGNAL(clicked()), this, SLOT(onRemoveSelectedForSearchButtonClicked()));
    connect(removeSelectedForKeyWordButton, SIGNAL(clicked()), this, SLOT(onRemoveSelectedForKeyWordButtonClicked()));
    connect(removeSelectedForProxyButton, SIGNAL(clicked()), this, SLOT(onRemoveSelectedForProxyButtonClicked()));
    connect(submitChangeForSearchButton, SIGNAL(clicked()), this, SLOT(onSubmitChangeForSearchButtonClicked()));
    connect(submitChangeForKeyWordButton, SIGNAL(clicked()), this, SLOT(onSubmitChangeForKeyWordButtonClicked()));
    connect(submitChangeForProxyButton, SIGNAL(clicked()), this, SLOT(onSubmitChangeForProxyButtonClicked()));
}

void ConfigDialog::onLoadForKeyWordButtonClicked()
{
    QString fileName = this->getFileName();
    if (fileName.isEmpty()) return;
    QFileInfo fileInfo(fileName);
    if (fileInfo.suffix().toLower() == "txt")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            showMessage("error to open " + fileName);
            return;
        }
        QTextStream in(&file);
        in.setCodec(QTextCodec::codecForName("System"));
        QVariantList words;
        bool result = true;
        while(!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            words << line;
            if (words.count() == 100) {//insert for every 100 words
                result = DBUtil::insertKeyWords(words);
                if (!result) {
                    showMessage("insert key word error");
                    file.close();
                    return;
                }
                words.clear();
            }
        }
        //insert outside
        if (!words.isEmpty()) {
            result = DBUtil::insertKeyWords(words);
            if (!result) {
                showMessage("insert key word error");
                file.close();
                return;
            }
        }
        file.close();
    }
    else if (fileInfo.suffix().toLower() == "xls")
    {
        showMessage("not support yet");
    }
    //update the view
    keyWordModel->select();

}
void ConfigDialog::onLoadForProxyButtonClicked()
{
    QString fileName = this->getFileName();
    if (fileName.isEmpty()) return;
    QFileInfo fileInfo(fileName);
    if (fileInfo.suffix().toLower() == "txt")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            showMessage("error to open " + fileName);
            return;
        }
        QTextStream in(&file);
        in.setCodec(QTextCodec::codecForName("System"));
        QVariantList ips;
        QVariantList ports;
        bool result = true;
        while(!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            QStringList words = line.split(QRegExp("\\s+|\\t+"));
            qDebug() << words;
            ips << words.at(0);
            ports << words.at(1).toInt();
            if (words.count() == 100) {//insert for every 100 words
                result = DBUtil::insertProxys(ips, ports);
                if (!result) {
                    showMessage("insert proxy error");
                    file.close();
                    return;
                }
                ips.clear();
                ports.clear();
            }
        }
        //insert outside
        if (!ips.isEmpty()) {
            result = DBUtil::insertProxys(ips, ports);
            if (!result) {
                showMessage("insert proxy error");
                file.close();
                return;
            }
        }
        file.close();
    }
    else if (fileInfo.suffix().toLower() == "xls")
    {
        showMessage("not support yet");
    }
    //update the view
    proxyModel->select();

}
void ConfigDialog::onLoadForSearchButtonClicked()
{
    QString fileName = this->getFileName();
    if (fileName.isEmpty()) return;
    QFileInfo fileInfo(fileName);
    if (fileInfo.suffix().toLower() == "txt")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            showMessage("error to open " + fileName);
            return;
        }
        QTextStream in(&file);
        in.setCodec(QTextCodec::codecForName("System"));
        QVariantList engineNames;
        QVariantList engineUrls;
        bool result = true;
        while(!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            QStringList words = line.split(QRegExp("\\s+|\\t+"));
            qDebug() << words;
            engineNames << words.at(0);
            engineUrls << words.at(1);
            if (engineNames.count() == 100) {//insert for every 100 words
                result = DBUtil::insertEngines(engineNames, engineUrls);
                if (!result) {
                    showMessage("insert engine error");
                    file.close();
                    return;
                }
                engineNames.clear();
                engineUrls.clear();
            }
        }
        //insert outside
        if (!engineNames.isEmpty()) {
            result = DBUtil::insertEngines(engineNames, engineUrls);
            if (!result) {
                showMessage("insert engine error");
                file.close();
                return;
            }
        }
        file.close();
    }
    else if (fileInfo.suffix().toLower() == "xls")
    {
        showMessage("not support yet");
    }
    //update the view
    searchEngineModel->select();

}
void ConfigDialog::onRemoveSelectedForKeyWordButtonClicked()
{
    QItemSelectionModel *selections = keyWordView->selectionModel();
    QModelIndexList indexes = selections->selectedIndexes();
    if (indexes.count() == 0) {
        showMessage("no row selected");
    }
    QVariantList ids;
    foreach(QModelIndex index, indexes)
    {
        if(index.column() == 0) {
            ids << index.data();
        }
    }
    bool result = DBUtil::deleteKeyWords(ids);
    if (! result) {
        showMessage("error delete key word");
    }
    else
        keyWordModel->select();
}
void ConfigDialog::onRemoveSelectedForProxyButtonClicked()
{
    QItemSelectionModel *selections = proxyView->selectionModel();
    QModelIndexList indexes = selections->selectedIndexes();
    if (indexes.count() == 0) {
        showMessage("no row selected");
    }
    QVariantList ids;
    foreach(QModelIndex index, indexes)
    {
        if(index.column() == 0) {
            ids << index.data();
        }
    }
    bool result = DBUtil::deleteProxys(ids);
    if (! result) {
        showMessage("error delete key word");
    }
    else
        proxyModel->select();
}
void ConfigDialog::onRemoveSelectedForSearchButtonClicked()
{
    QItemSelectionModel *selections = searchEngineView->selectionModel();
    QModelIndexList indexes = selections->selectedIndexes();
    if (indexes.count() == 0) {
        showMessage("no row selected");
    }
    QVariantList ids;
    foreach(QModelIndex index, indexes)
    {
        if(index.column() == 0) {
            ids << index.data();
        }
    }
    bool result = DBUtil::deleteEngines(ids);
    if (! result) {
        showMessage("error delete engine");
    }
    else
        searchEngineModel->select();
}

void ConfigDialog::onSubmitChangeForKeyWordButtonClicked()
{
    bool result = keyWordModel->submitAll();
    if (!result)
    {
        showMessage("get exception:" + keyWordModel->lastError().text());
    }
    else {
        showSuccessMessage("Sucess Submit");
    }
}
void ConfigDialog::onSubmitChangeForProxyButtonClicked()
{
    bool result = proxyModel->submitAll();
    proxyModel->database().commit();
    if (!result)
    {
        showMessage("get exception:" + proxyModel->lastError().text());
    }
    else {
        showSuccessMessage("Sucess Submit");
    }
}
void ConfigDialog::onSubmitChangeForSearchButtonClicked()
{
    bool result = searchEngineModel->submitAll();
    if (!result)
    {
        showMessage("get exception:" + searchEngineModel->lastError().text());
    }
    else
    {
        showSuccessMessage("Sucess Submit");
    }
}

QString ConfigDialog::getFileName()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "./",
                                                    tr("File (*.txt *.xls)"));
    qDebug() << "get filename: " << fileName;
    return fileName;
}

void ConfigDialog::showMessage(const QString& msg)
{
    QMessageBox::information(this, tr("error"), msg);
}
void ConfigDialog::showSuccessMessage(const QString &msg)
{
    QMessageBox::information(this, tr("Success"), msg);
}
