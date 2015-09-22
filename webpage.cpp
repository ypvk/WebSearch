#include "webpage.h"
#include <QNetworkRequest>
#include <QDebug>

WebPage::WebPage(QWidget *parent) :
    QWebPage(parent)
{
}
bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type){
    if(type==0){//如果是用户点击
        if(frame!=mainFrame()){ //如果不是在本窗口的连接
            emit openLink(request.url());//发送打开新连接信号
        }else{
            emit loadLink(request.url());//发送点击连接信号
        }

    }
    else {
        qDebug() << "other===================";
    }
    return QWebPage::acceptNavigationRequest(frame, request, type);
}
