#include "browser.h"
#include "webpage.h"
#include "networkcookiejar.h"
#include "updateinfo.h"
#include "commonutils.h"
#include "clickinfo.h"
#include "engineinfo.h"
#include <QtGui>
#include <QtWebKit>
#include <QTabWidget>
#include <QTimer>

Browser::Browser(QWidget *parent, int id):
    QMainWindow(parent)
{
    this->id = id;
    init();
}
void Browser::init()
{
    initConfig();
    shouldBack = false;
    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    file.close();
    progress = 0;

    mutex = new QMutex;
    list = new QList<int>();

    tabwidget = new QTabWidget(this);

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    cookieJar = new NetWorkCookieJar(this);

    view = new QWebView(this);
    tabwidget->addTab((QWidget*)view, tr("main view"));
    WebPage* webPage = new WebPage(view);
    view->setPage(webPage);
    view->page()->networkAccessManager()->setCookieJar(cookieJar);

    QObject::connect(webPage, SIGNAL(loadLink(QUrl)), this, SLOT(loadUrl(QUrl)));
    QObject::connect(webPage, SIGNAL(openLink(QUrl)), this, SLOT(openLink(QUrl)));

    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    locationEdit = new QLineEdit();
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));
    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));
    toolBar->addAction(view->pageAction(QWebPage::Reload));
    toolBar->addAction(view->pageAction(QWebPage::Stop));
    toolBar->addWidget(locationEdit);
    toolBar->addSeparator();

    keyWordEdit = new QLineEdit(this);
    keyWordEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    keyWordEdit->setMaximumWidth(100);
    connect(keyWordEdit, SIGNAL(returnPressed()), this, SLOT(startSearch()));
//    toolBarKeyWord->addWidget(keyWordEdit);
    toolBar->addWidget(keyWordEdit);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QAction* viewSourceAction = new QAction("Page Source", this);
    connect(viewSourceAction, SIGNAL(triggered()), SLOT(viewSource()));
    viewMenu->addAction(viewSourceAction);

    QMenu *effectMenu = menuBar()->addMenu(tr("&Effect"));
    effectMenu->addAction("Highlight all links", this, SLOT(highlightAllLinks()));

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));

    setCentralWidget(tabwidget);
    setUnifiedTitleAndToolBarOnMac(true);

    connect(this, SIGNAL(searchFinished()), this, SLOT(onSearchFinished()));

//    view->load(QUrl("http://www.baidu.com"));
    keyWordEdit->setEnabled(false);
    isStop = true;
    //set qtimer
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkIfLoadFinished()));
    connect(view, SIGNAL(loadFinished(bool)), timer, SLOT(stop()));
}

Browser::Browser(QWidget *parent) :
        QMainWindow(parent)
{
    init();
//    initConfig();
//    shouldBack = false;
//    QFile file;
//    file.setFileName(":/jquery.min.js");
//    file.open(QIODevice::ReadOnly);
//    jQuery = file.readAll();
//    file.close();
//    progress = 0;

//    mutex = new QMutex;
//    list = new QList<int>();

//    tabwidget = new QTabWidget(this);

//    QNetworkProxyFactory::setUseSystemConfiguration(true);

//    cookieJar = new NetWorkCookieJar(this);

//    view = new QWebView(this);
//    tabwidget->addTab((QWidget*)view, tr("main view"));
//    WebPage* webPage = new WebPage(view);
//    view->setPage(webPage);
//    view->page()->networkAccessManager()->setCookieJar(cookieJar);

//    QObject::connect(webPage, SIGNAL(loadLink(QUrl)), this, SLOT(loadUrl(QUrl)));
//    QObject::connect(webPage, SIGNAL(openLink(QUrl)), this, SLOT(openLink(QUrl)));

//    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
//    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
//    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
//    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

//    locationEdit = new QLineEdit();
//    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
//    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));
//    QToolBar *toolBar = addToolBar(tr("Navigation"));
//    toolBar->addAction(view->pageAction(QWebPage::Back));
//    toolBar->addAction(view->pageAction(QWebPage::Forward));
//    toolBar->addAction(view->pageAction(QWebPage::Reload));
//    toolBar->addAction(view->pageAction(QWebPage::Stop));
//    toolBar->addWidget(locationEdit);
//    toolBar->addSeparator();

//    keyWordEdit = new QLineEdit(this);
//    keyWordEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
//    keyWordEdit->setMaximumWidth(100);
//    connect(keyWordEdit, SIGNAL(returnPressed()), this, SLOT(startSearch()));
////    toolBarKeyWord->addWidget(keyWordEdit);
//    toolBar->addWidget(keyWordEdit);

//    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
//    QAction* viewSourceAction = new QAction("Page Source", this);
//    connect(viewSourceAction, SIGNAL(triggered()), SLOT(viewSource()));
//    viewMenu->addAction(viewSourceAction);

//    QMenu *effectMenu = menuBar()->addMenu(tr("&Effect"));
//    effectMenu->addAction("Highlight all links", this, SLOT(highlightAllLinks()));

//    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));

//    setCentralWidget(tabwidget);
//    setUnifiedTitleAndToolBarOnMac(true);

//    connect(this, SIGNAL(searchFinished()), this, SLOT(onSearchFinished()));

////    view->load(QUrl("http://www.baidu.com"));
//    keyWordEdit->setEnabled(false);
//    isStop = true;
//    //set qtimer
//    timer = new QTimer(this);
//    timer->setSingleShot(true);
//    connect(timer, SIGNAL(timeout()), this, SLOT(checkIfLoadFinished()));
//    connect(view, SIGNAL(loadFinished(bool)), timer, SLOT(stop()));
}
void Browser::viewSource()
{
    QNetworkAccessManager* accessManager = view->page()->networkAccessManager();
    QNetworkRequest request(view->url());
    QNetworkReply* reply = accessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(slotSourceDownloaded()));
}

void Browser::slotSourceDownloaded()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(const_cast<QObject*>(sender()));
    QTextEdit* textEdit = new QTextEdit(NULL);
    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    textEdit->show();
    textEdit->setPlainText(reply->readAll());
    reply->deleteLater();
}

void Browser::adjustLocation()
{
    locationEdit->setText(view->url().toString());
}

void Browser::changeLocation()
{
    QUrl url = QUrl(locationEdit->text());
    view->load(url);
    view->setFocus();
    keyWordEdit->setEnabled(false);
}
void Browser::adjustTitle()
{
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void Browser::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

void Browser::finishLoading(bool)
{
    progress = 100;
    adjustTitle();
    view->page()->mainFrame()->evaluateJavaScript(jQuery);

    keyWordEdit->setEnabled(true);
    if (searchFlag)
    {
        searchFlag = false;
        timer->stop();
        startSearch();
    }
}
void Browser::highlightAllLinks()
{
    QString code = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void Browser::openLink(const QUrl &url)
{
    QWebView* m_webView = new QWebView(this);
    WebPage* m_webPage = new WebPage(m_webView);
    m_webView->setPage(m_webPage);
    m_webView->load(url);
    mutex->lock();
    int index = tabwidget->addTab((QWidget*)m_webView, tr("triger view"));
    list->append(index);
    mutex->unlock();
    QTimer::singleShot(3000, this, SLOT(onTabTimeOut()));
    tabwidget->setCurrentIndex(index);
}

void Browser::loadUrl(const QUrl &url)
{
    if (!url.toString().contains("search.yahoo.com"))
    {
        view->load(url);
        keyWordEdit->setEnabled(false);
    }
    if (shouldBack) {
        QTimer::singleShot(3000, this, SLOT(onLinkTimeOut()));
    }
}

void Browser::onTabTimeOut()
{
    mutex->lock();
    searchFlag = false;
    view->stop();
    int index = list->first();
    list->removeFirst();
    QWidget* webView = tabwidget->widget(index);
    webView->close();
    delete webView;
    tabwidget->removeTab(index);
    mutex->unlock();
    emit searchFinished();
}

void Browser::onLinkTimeOut()
{
//    view->back();//don't back
//  CommonUtils::sleep(3000);
    searchFlag = false;
    shouldBack = false;
    //set before stop
    view->stop();//stop recieve data
    qDebug() << "link clicked emit searchFinished";
    emit searchFinished();
}

void Browser::startSearch()
{
   QString url = currentUrl;
   if (url.endsWith("/")) url = url.left(url.size() - 1);
   if (engineConfigMap.contains(url))
       searchEngineKey = url;
   else
   {
       qDebug() << "not set in config.ini set " << url <<" into file";
       searchEngineKey = "default";
   }
   startFillKeyWord();
}

void Browser::startFillKeyWord()
{
    QString textSelecter = engineConfigMap[searchEngineKey].inputText;
    QString keyWord = keyWordEdit->text();
    QWebElement element = view->page()->mainFrame()->findFirstElement(textSelecter);
    element.setAttribute("value", keyWord);
    QTimer::singleShot(1000, this, SLOT(startSubmit()));
}

void Browser::startSubmit()
{
    QString submitSelector = engineConfigMap[searchEngineKey].inputSubmit;
    QWebElement element = view->page()->mainFrame()->findFirstElement(submitSelector);
    qDebug() << element.geometry().center();
    QPoint elemPos = element.geometry().center();
    buttonClick(elemPos);
    QTimer::singleShot(4000, this, SLOT(startHrefClick()));
}

void Browser::startHrefClick()
{
    QString hrefSelector = engineConfigMap[searchEngineKey].hrefLink;
    baseHrefClick(hrefSelector);

}


Browser::~Browser()
{
    delete mutex;
    delete list;
}

void Browser::baseHrefClick(const QString &lickItemSelector)
{
    QWebElementCollection elements = view->page()->mainFrame()->findAllElements(lickItemSelector);
    if (elements.count() == 0) {//don't have elements just return
        qDebug() << "href none emit searchFinished";
        emit searchFinished();
        return;
    }
    int index = CommonUtils::rand(elements.count());
    qDebug() << "select element " << index;
    QWebElement element = elements[index];
    qDebug() << "click element " << element.toPlainText();
    currentLinkName = element.toPlainText();
    currentLinkUrl = element.attribute("href", "null");
    QWebFrame* frame = view->page()->mainFrame();
    qDebug() << element.toPlainText();
    const QPoint elemPos=element.geometry().center();
    QPoint scrollPosition = QPoint(0, elemPos.y() - 100);
    frame->setScrollPosition(scrollPosition);
    QPoint const scrollPos=frame->scrollPosition();

    buttonClick(elemPos-scrollPos);
    shouldBack = true;
}

void Browser::buttonClick(const QPoint& pos)
{
    QMouseEvent * const impossibleMousePress = new QMouseEvent(QEvent::MouseButtonPress,pos,Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QMouseEvent * const impossibleMouseRelease = new QMouseEvent(QEvent::MouseButtonRelease,pos,Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(view,impossibleMousePress);
    QApplication::postEvent(view,impossibleMouseRelease);
}

void Browser::clearCookie()
{
    QList<QNetworkCookie>  empty = QList<QNetworkCookie>();
    cookieJar->setCookies(empty);
}

void Browser::search(const QList<ClickInfo>& clickInfos)
{
    this->clickInfos.clear();//clear
    this->clickInfos = clickInfos;
    this->currentEngine = "";
    this->currentUrl = "";
    this->currentKeyWord = "";
    this->currentLinkName = "";
    this->currentLinkUrl = "";
    this->currentIp = "";
    this->currentWordIndex = 0;
    this->currentClickNum = 0;

    isStop = false;
    emit searchFinished();
}
void Browser::onSearchFinished()
{
    timer->stop();
    checkAndEmitRealtimeInfo();
    clearCookie();
    if (isStop || clickInfos.isEmpty())
    {
        isStop = true;
        emit jobFinished(id);
        return;
    }
    //not empty search again build search key word
    EngineInfo engineInfo = clickInfos.first().getEngineInfo();
    QString engine = engineInfo.getEngineName();
    QString url = engineInfo.getEngineUrl();
    QList<QPair<QString, int> > proxys = clickInfos.first().getProxys();
    currentWordIndex = currentWordIndex < clickInfos.first().getKeyWords().size() ? currentWordIndex : 0;
    QString keyWord = clickInfos.first().getKeyWords().at(currentWordIndex++);
    //set current
    currentEngine = engine;
    currentUrl = url;
    currentKeyWord = keyWord;
    currentLinkName = "";
    currentLinkUrl = "";
    //set proxy
//    int index = proxys.count() - currentClickNum % (proxys.count() + 1);
    if (0 == proxys.count())
    {//use no proxy
        currentIp = CommonUtils::getMyIp();
        qDebug() << "use local ip->" << currentIp;
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::NoProxy);
        view->page()->networkAccessManager()->setProxy(proxy);
    }
    else
    {
        int index = currentClickNum % proxys.count();
        QString hostname = proxys.at(index).first;
        int port = proxys.at(index).second;
        qDebug() << "use proxy->" << hostname << ":" << port;
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(hostname);
        proxy.setPort(port);
        view->page()->networkAccessManager()->setProxy(proxy);
        currentIp = hostname;
    }
    //load url
    searchFlag = true;
    view->load(currentUrl);
    keyWordEdit->setEnabled(true);
    keyWordEdit->setText(currentKeyWord);
    //set timeout for the url is not load successfull
    timer->start(3000);
}

void Browser::checkAndEmitRealtimeInfo()
{
    if (currentUrl != "") {//click successfull
        if (clickInfos.isEmpty()) return;//return if the clickinfos is empty
        UpdateInfo updateInfo(currentEngine,
                              currentUrl,
                              currentKeyWord,
                              currentLinkName,
                              currentLinkUrl,
                              currentIp);
        currentClickNum++;
        int clickNum = clickInfos.first().getClickNum();
        int totalClickNum = clickNum * clickInfos.first().getKeyWords().count() * clickInfos.first().getProxys().count();
        qDebug() << "clickNum:" << currentClickNum;
        qDebug() << "totalCliclNum:" << totalClickNum;
        if (currentClickNum == totalClickNum)
        {
            //if currentClickNum == totalClickNum clickInfos should move next
            clickInfos.removeFirst();
            //reset current click num and current word index
            currentClickNum = 0;
            currentWordIndex = 0;
        }
        emit updateClickInfo(updateInfo);
    }
}

void Browser::stopSearch()
{
    isStop = true;
    //TODO check if stop the view
    searchFlag = false;
    view->stop();
}
void Browser::checkIfLoadFinished()
{
    if (searchFlag != false)
    {
        searchFlag = false;
        view->stop();
        qDebug() << "check add searchFinished";
        emit this->searchFinished();
    }
}

void Browser::initConfig()
{
    this->engineConfigMap = CommonUtils::getEngineConfigs();
    qDebug() << engineConfigMap.keys();
//    qDebug() << engineConfigMap["default"].hrefLink;
}
