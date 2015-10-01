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

Browser::Browser(QWidget *parent) :
        QMainWindow(parent)
{
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

    rotateAction = new QAction(this);
    rotateAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    rotateAction->setCheckable(true);
    rotateAction->setText(tr("Turn images upside down"));
    connect(rotateAction, SIGNAL(toggled(bool)), this, SLOT(rotateImages(bool)));
    effectMenu->addAction(rotateAction);

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));

    setCentralWidget(tabwidget);
    setUnifiedTitleAndToolBarOnMac(true);

    connect(this, SIGNAL(searchFinished()), this, SLOT(onSearchFinished()));

//    view->load(QUrl("http://www.baidu.com"));
    keyWordEdit->setEnabled(false);
    isStop = true;

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
        startSearch();
    }
}
void Browser::highlightAllLinks()
{
    QString code = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void Browser::rotateImages(bool invert)
{
    QString code;
    if (invert)
        code = "$('img').each( function () { $(this).css('-webkit-transition', '-webkit-transform 2s'); $(this).css('-webkit-transform', 'rotate(180deg)') } )";
    else
        code = "$('img').each( function () { $(this).css('-webkit-transition', '-webkit-transform 2s'); $(this).css('-webkit-transform', 'rotate(0deg)') } )";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void Browser::removeGifImages()
{
    QString code = "$('[src*=gif]').remove()";
    view->page()->mainFrame()->evaluateJavaScript(code);
}
void Browser::removeInlineFrames()
{
    QString code = "$('iframe').remove()";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void Browser::removeObjectElements()
{
    QString code = "$('object').remove()";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void Browser::removeEmbeddedElements()
{
    QString code = "$('embed').remove()";
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
    QTimer::singleShot(5000, this, SLOT(onTabTimeOut()));
    tabwidget->setCurrentIndex(index);
}

void Browser::loadUrl(const QUrl &url)
{
    view->load(url);
    keyWordEdit->setEnabled(false);
    if (shouldBack) {
        QTimer::singleShot(5000, this, SLOT(onLinkTimeOut()));
    }
}

void Browser::onTabTimeOut()
{
    mutex->lock();
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
    view->back();
//    CommonUtils::sleep(3000);
    shouldBack = false;
    emit searchFinished();
}

void Browser::startSearch()
{
   QString url = view->url().toString();
   if (url.startsWith(tr("http://m.baidu.com")))
       startSearchForMBaidu();
   else if (url.startsWith("https://www.baidu.com") || url.startsWith("http://www.baidu.com"))
       startSearchForBaidu();
}

void Browser::startSearchForBaidu()
{
    QString inTextSelector = "#kw";
    QString keyWord = keyWordEdit->text();
    QWebElement element = view->page()->mainFrame()->findFirstElement(inTextSelector);
    element.setAttribute("value", keyWord);
    QTimer::singleShot(3000, this, SLOT(baiduSubmitButtonClick()));
}

void Browser::startSearchForMBaidu()
{
    QString inTextSelector = "input[type='text']";
    QString keyWord = keyWordEdit->text();
    QWebElement element = view->page()->mainFrame()->findFirstElement(inTextSelector);
    element.setAttribute("value", keyWord);
    QTimer::singleShot(3000, this, SLOT(mBaiduSubmitButtonClick()));
}

void Browser::baseSearchAction(const QString &textSelector, const QString &submitSelector)
{
    QString inTextSelector = textSelector.isEmpty() ? "input[type='text']" : textSelector;
    QString inSubmitSelector = submitSelector.isEmpty() ? "input[type=submit]" : submitSelector;
    QString keyWord = keyWordEdit->text();
    QWebElement element = view->page()->mainFrame()->findFirstElement(inTextSelector);
    element.setAttribute("value", keyWord);

    element = view->page()->mainFrame()->findFirstElement(inSubmitSelector);
    qDebug() << element.geometry().center();
    QPoint elemPos = element.geometry().center();
    buttonClick(elemPos);
}

void Browser::baiduSubmitButtonClick()
{
    QString inSubmitSelector = "input[type=submit]";
    QWebElement element = view->page()->mainFrame()->findFirstElement(inSubmitSelector);
    qDebug() << element.geometry().center();
    QPoint elemPos = element.geometry().center();
    buttonClick(elemPos);
    QTimer::singleShot(5000, this, SLOT(hrefClick()));
}

void Browser::mBaiduSubmitButtonClick()
{
    QString inSubmitSelector = "input[type=submit]";
    QWebElement element = view->page()->mainFrame()->findFirstElement(inSubmitSelector);
    qDebug() << element.geometry().center();
    QPoint elemPos = element.geometry().center();
    buttonClick(elemPos);
    QTimer::singleShot(5000, this, SLOT(mHrefClick()));
}

Browser::~Browser()
{
    delete mutex;
    delete list;
}

void Browser::hrefClick()
{
    baseHrefClick(".c-container .t a[href]");
}
void Browser::mHrefClick()
{
    baseHrefClick(".resitem a[href]");
    shouldBack = true;

}

void Browser::baseHrefClick(const QString &lickItemSelector)
{
    QWebElementCollection elements = view->page()->mainFrame()->findAllElements(lickItemSelector);
    if (elements.count() == 0) {//don't have elements just return
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
    checkAndEmitRealtimeInfo();
    clearCookie();
    if (isStop || clickInfos.isEmpty())
    {
        isStop = true;
        emit jobFinished();
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
    //set proxy
    int index = proxys.count() - currentClickNum % (proxys.count() + 1);
    if (index == proxys.count())
    {//use no proxy
        currentIp = CommonUtils::getMyIp();
        qDebug() << "use local ip->" << currentIp;
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::NoProxy);
        view->page()->networkAccessManager()->setProxy(proxy);
    }
    else
    {
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
//    CommonUtils::sleep(5000);
//    startSearch();
}

void Browser::checkAndEmitRealtimeInfo()
{
    if (currentUrl != "") {//click successfull
        UpdateInfo updateInfo(currentEngine,
                              currentUrl,
                              currentKeyWord,
                              currentLinkName,
                              currentLinkUrl,
                              currentIp);
        currentClickNum++;
        int clickNum = clickInfos.first().getClickNum();
        int totalClickNum = clickNum * clickInfos.first().getKeyWords().count();
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
    //view->stop();
}
