#include "configdialog.h"
#include "dbutil.h"
#include <QtGui>
#include <QtSql>
#include <QSettings>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent)
{
    setupGui();
    setupModel();
    setupConnection();
    loadSettings();
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

    loadForSearchButton = new QPushButton(tr("导入"), this);
    loadForKeyWordButton = new QPushButton(tr("导入"), this);
    loadForProxyButton = new QPushButton(tr("导入"), this);
    removeSelectedForSearchButton = new QPushButton(tr("删除选中"), this);
    removeSelectedForKeyWordButton = new QPushButton(tr("删除选中"), this);
    removeSelectedForProxyButton = new QPushButton(tr("删除选中"), this);
    submitChangeForSearchButton = new QPushButton(tr("提交更改"), this);
    submitChangeForKeyWordButton = new QPushButton(tr("提交更改"), this);
    submitChangeForProxyButton = new QPushButton(tr("提交更改"), this);
    clearSearchButton = new QPushButton(tr("清空"), this);
    clearKeyButton = new QPushButton(tr("清空"), this);
    clearProxyButton = new QPushButton(tr("清空"), this);


    searchEngineGroupBox = new QGroupBox(tr("搜索引擎"), this);
    searchEngineOperationBox = new QGroupBox(tr("操作"), this);

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
    seOperationBoxLayout->addWidget(clearSearchButton);
    seOperationBoxLayout->addWidget(submitChangeForSearchButton);
    searchEngineOperationBox->setLayout(seOperationBoxLayout);


    keyWordGroupBox = new QGroupBox(tr("关键词"), this);
    keyWordOperationBox = new QGroupBox(tr("操作"), this);

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
    kwOperationBoxLayout->addWidget(clearKeyButton);
    kwOperationBoxLayout->addWidget(submitChangeForKeyWordButton);
    keyWordOperationBox->setLayout(kwOperationBoxLayout);
    //proxy
    proxyGroupBox = new QGroupBox(tr("代理"), this);
    proxyOperationBox = new QGroupBox(tr("操作"), this);

    QVBoxLayout* proxyWidgetLayout = new QVBoxLayout;
    proxyWidgetLayout->addWidget(proxyGroupBox);
    proxyWidgetLayout->addWidget(proxyOperationBox);
    proxyWidget->setLayout(proxyWidgetLayout);

    QVBoxLayout* pxGroupBoxLayout = new QVBoxLayout;
    pxGroupBoxLayout->addWidget(proxyView);
    proxyGroupBox->setLayout(pxGroupBoxLayout);
    radioButton = new QRadioButton(tr("API"));
    proxyApiValue = new QLineEdit(this);
    QHBoxLayout* proxyApiLayout = new QHBoxLayout;
    proxyApiLayout->addWidget(radioButton);
    proxyApiLayout->addWidget(proxyApiValue);
    pxGroupBoxLayout->addLayout(proxyApiLayout);


    QHBoxLayout* pxOperationBoxLayout = new QHBoxLayout;
    pxOperationBoxLayout->addWidget(loadForProxyButton);
    pxOperationBoxLayout->addWidget(removeSelectedForProxyButton);
    pxOperationBoxLayout->addWidget(clearProxyButton);
    pxOperationBoxLayout->addWidget(submitChangeForProxyButton);
    proxyOperationBox->setLayout(pxOperationBoxLayout);


    mainWidget->addTab(searchEngineWidget, tr("搜索引擎配置"));
    mainWidget->addTab(keyWordWidget, tr("关键词配置"));
    mainWidget->addTab(proxyWidget, tr("代理配置"));

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
    keyWordModel->setHeaderData(1, Qt::Horizontal, QObject::tr("MainKey"));
    keyWordModel->setHeaderData(1, Qt::Horizontal, QObject::tr("AssistKey"));
    keyWordView->setModel(keyWordModel);

    proxyModel = new QSqlTableModel(this, db);
    proxyModel->setTable(DBUtil::PROXY_NAME);
    proxyModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    proxyModel->select();
    proxyModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    proxyModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Url"));
    proxyModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Port"));
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
    connect(radioButton, SIGNAL(toggled(bool)), this, SLOT(onRaidoButtonToogled(bool)));
    connect(clearProxyButton, SIGNAL(clicked()), this, SLOT(onClearProxyButton()));
    connect(clearSearchButton, SIGNAL(clicked()), this, SLOT(onClearSearchButtonClicked()));
    connect(clearKeyButton, SIGNAL(clicked()), this, SLOT(onClearKeyButtonClicked()));
}

void ConfigDialog::onRaidoButtonToogled(bool state)
{
    if (state == true) {
        proxyView->setEnabled(false);
        proxyApiValue->setEnabled(true);
    }
    else {
        proxyView->setEnabled(true);
        proxyApiValue->setEnabled(false);
    }
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
        QVariantList mainKeys;
        QVariantList assistKeys;
        bool result = true;
        while(!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            QStringList words = line.split(QRegExp("\\s+|\\t+"));
            mainKeys << words.at(0);
            assistKeys << words.at(1);
            if (mainKeys.count() == 100) {//insert for every 100 words
                result = DBUtil::insertKeyWords(mainKeys, assistKeys);
                if (!result) {
                    showMessage("insert key word error");
                    file.close();
                    return;
                }
                mainKeys.clear();
                assistKeys.clear();
            }
        }
        //insert outside
        if (!mainKeys.isEmpty()) {
            result = DBUtil::insertKeyWords(mainKeys, assistKeys);
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
            QStringList words = line.split(QRegExp("\\s+|\\t+|:|："));
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
void ConfigDialog::onClearKeyButtonClicked()
{
    bool result = DBUtil::clearTable(DBUtil::KEY_WORD_NAME);
    if (! result)
    {
        showMessage("ERROR clear data");
    }
    else {
        keyWordModel->select();
    }
}
void ConfigDialog::onClearProxyButton()
{
    bool result = DBUtil::clearTable(DBUtil::PROXY_NAME);
    if (! result)
    {
        showMessage("ERROR clear data");
    }
    else {
        proxyModel->select();
    }
}
void ConfigDialog::onClearSearchButtonClicked()
{
    bool result = DBUtil::clearTable(DBUtil::SEARCH_ENGINE_NAME);
    if (! result)
    {
        showMessage("ERROR clear data");
    }
    else
    {
        searchEngineModel->select();
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
void ConfigDialog::loadSettings()
{
    bool radio = settings.value("proxyApi/radio").toBool();
    QString value = settings.value("proxyApi/value").toString();
    qDebug() << " load "<<radio;
    radioButton->setChecked(radio);
    proxyApiValue->setText(value);
}
void ConfigDialog::saveSettings()
{
    bool radio = radioButton->isChecked();
    QString value = proxyApiValue->text();
    qDebug() << "save" << radio;
    settings.setValue("proxyApi/radio", radio);
    settings.setValue("proxyApi/value", value);
}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

bool ConfigDialog::isUseProxyApi()
{
    return radioButton->isChecked();
}
QString ConfigDialog::getProxyApiValue()
{
    return proxyApiValue->text();
}
