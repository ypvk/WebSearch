#include "webpage.h"
#include "browser.h"
#include <QNetworkRequest>
#include <QDebug>

WebPage::WebPage(QWidget *parent) :
    QWebPage(parent)
{
}
WebPage::WebPage(Browser *parent):
        QWebPage(parent)
{
    this->browser = parent;
}

bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type){
    if(type==0){//如果是用户点击
        qDebug() << "user click==========";
        qDebug() << request.url().toString();
        if(frame!=mainFrame()){ //如果不是在本窗口的连接
            emit openLink(request.url());//发送打开新连接信号
        }else{
            emit loadLink(request.url());//发送点击连接信号
        }

    }
    else if (type == QWebPage::NavigationTypeFormSubmitted) {
        qDebug() << "form submit==================";
        QUrl newUrl(request.url());
        if (browser->getQueryState()) {
            newUrl.addQueryItem("rq", browser->getCurrentKeyWord().second);
        } else {
            newUrl.addQueryItem("rq", browser->getCurrentKeyWord().first);
        }
        qDebug() << newUrl.toString();
        emit loadLink(newUrl);
    }
    else {
        qDebug() << "other===================" << type;
        qDebug() << request.url().toString();
        if (request.url().toString().contains("search.yahoo.com"))
        {
            qDebug() << "exit url " << request.url().toString();
            return false;
        }
        else
        {
            return QWebPage::acceptNavigationRequest(frame, request, type);
        }
    }
    return false;

}

void WebPage::javaScriptAlert(QWebFrame *originatingFrame, const QString &msg)
{
    return;
}
bool WebPage::javaScriptConfirm(QWebFrame *originatingFrame, const QString &msg)
{
    return false;
}
bool WebPage::javaScriptPrompt(QWebFrame *originatingFrame, const QString &msg, const QString &defaultValue, QString *result)
{
    return false;
}

QString WebPage::userAgentForUrl(const QUrl &url) const
{
    return "MQQBrowser/26 Mozilla/5.0 (Linux; U; Android 2.3.7; zh-cn; MB200 Build/GRJ22; CyanogenMod-7) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1";
}
