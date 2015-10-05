#include "webpage.h"
#include <QNetworkRequest>
#include <QDebug>

WebPage::WebPage(QWidget *parent) :
    QWebPage(parent)
{
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
    else {
        qDebug() << "other===================" << type;
        qDebug() << request.url().toString();
    }
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
