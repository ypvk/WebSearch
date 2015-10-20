#include "querythread.h"
#include "networkcookiejar.h"
#include "commonutils.h"
#include "clickinfo.h"
#include "updateinfo.h"
#include <QtNetwork>
#include <QtCore>

QueryThread::QueryThread(QObject *parent) :
    QThread(parent)
{
}
QueryThread::QueryThread(QObject *parent, const QList<ClickInfo> &clickInfos, int id): QThread(parent)
{
    this->clickInfos = clickInfos;
    this->id = id;
}
QueryThread::~QueryThread()
{
    delete this->networkManager;
    delete this->cookieJar;
    delete this->timer;
}

void QueryThread::run()
{
    this->networkManager = new QNetworkAccessManager();
    this->cookieJar = new NetWorkCookieJar(NULL);
    this->networkManager->setCookieJar(this->cookieJar);
    this->timer = new QTimer();
    this->timer->setSingleShot(true);
    QNetworkRequest request;
    for(int i = 0; i < clickInfos.count(); i++) {
        ClickInfo clickInfo = clickInfos.at(i);
        const QList<QPair<QString, int> >& proxys = clickInfo.getProxys();
        const QList<QPair<QString, QString> >& keyWords = clickInfo.getKeyWords();
        for(int j = 0; j < proxys.count(); j++) {
            QString hostname = proxys.at(j).first;
            int port = proxys.at(j).second;
            qDebug() << "use proxy->" << hostname << ":" << port;
            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(hostname);
            proxy.setPort(port);
            this->networkManager->setProxy(proxy);
            lastUrl = "";
            //clear cookie
            this->clearCookies();
            for(int m = 0; m < keyWords.count(); m++) {
                const QPair<QString, QString>& keyWord = keyWords.at(m);
                for(int k = 0; k < 50; k++) {
                    int resultCode = this->queryWord(keyWord.first, keyWord.second, request, hostname);
                    if (resultCode == 1) {
                        qDebug() << "thread:" << id
                                << " " << hostname
                                << " not get result value, current times:" << k;
                        continue;
                    }
                    else if (resultCode == 2) {
                        qDebug() << "thread:" << id
                                << " " << hostname
                                << " not accessable, current times:" << k;
                        break;
                    }
                    this->sleep(2000);
                    resultCode = this->queryWord(keyWord.second, keyWord.first, request, hostname);
                    if (resultCode == 1) {
                        qDebug() << "thread:" << id
                                << " " << hostname
                                << " not get result value, current times:" << k;
                        continue;
                    }
                    else if (resultCode == 2) {
                        qDebug() << "thread:" << id
                                << " " << hostname
                                << " not accessable, current times:" << k;
                        break;
                    }
                }
            }
        }
    }
}
int QueryThread::queryWord(QString mainWord, QString assistWord, QNetworkRequest &request, QString currentIp)
{
    QString hostName = "m.baidu.com";
    qDebug() << "thread: " << id << " query: " << mainWord << " " << assistWord;
    QEventLoop loop;
    int resultCode = 0;
    QString url = this->buildUrl(mainWord, assistWord);
    request.setUrl(url);
    this->setRequestHeader(request, hostName, lastUrl);
    lastUrl = url;
    QNetworkReply* reply = networkManager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer->start(3000);
    loop.exec();
    if (reply->isFinished())
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QString value = reply->readAll();
            qDebug() << value.left(100);
            qDebug() << "===get request value in thread:"  << id << " ==";
            QString currentEngine = "手机百度";
            QString currentUrl = "http://m.baidu.com";
            QPair<QString, QString> currentKeyWord(mainWord, assistWord);
            QString currentLinkName = "get link";
            QString currentLinkUrl = url;
            UpdateInfo updateInfo(currentEngine,
                                  currentUrl,
                                  currentKeyWord,
                                  currentLinkName,
                                  currentLinkUrl,
                                  currentIp);
            emit updateClickInfo(updateInfo);
        }
        else
        {
            resultCode = 1;
        }
    } else {
        if (this->networkManager->networkAccessible() == QNetworkAccessManager::NotAccessible) resultCode = 2;
    }
    reply->deleteLater();
    return resultCode;
}

void QueryThread::clearCookies()
{
    QList<QNetworkCookie>  empty = QList<QNetworkCookie>();
    cookieJar->setCookies(empty);
}

void QueryThread::setRequestHeader(QNetworkRequest &request, QString host, QString refer)
{
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    //request.setRawHeader("Accept-Encoding", "gzip, deflate, sdch");
    request.setRawHeader("Accept-Language", "h-CN,zh;q=0.8");
    request.setRawHeader("Host", host.toLocal8Bit());
    request.setRawHeader("Referer", refer.toLocal8Bit());
}

QString QueryThread::buildUrl(QString mainWord, QString assistWord)
{
    int randTs = CommonUtils::rand(1223145) + 100000;
    QString url = QString("http://m.baidu.com/s?word=%1&ts=%2&rq=%3").arg(mainWord).arg(randTs).arg(assistWord);
    qDebug() << "get url :" << url;
    return url;
}
