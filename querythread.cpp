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
}

void QueryThread::run()
{
    qRegisterMetaType<UpdateInfo>("UpdateInfo");
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
//            //-------------------
//            QString currentIp = CommonUtils::getMyIp();
//            hostname = currentIp;
//            //-------------------
            qDebug() << "use proxy->" << hostname << ":" << port;
            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(hostname);
            proxy.setPort(port);
//            proxy.setType(QNetworkProxy::NoProxy);
            this->networkManager->setProxy(proxy);
            lastUrl = "";
            //clear cookie
            this->clearCookies();
            for(int m = 0; m < keyWords.count(); m++) {
                const QPair<QString, QString>& keyWord = keyWords.at(m);
                for(int k = 0; k < 20; k++) {
                    this->sleep(3);
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
                    this->sleep(3);
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
                    qDebug() << "thread:" << id
                            << " " << hostname
                            << " recieve data, current times:" << k;
                }
            }
        }
    }
    delete this->networkManager;
    delete this->cookieJar;
    delete this->timer;
}
int QueryThread::queryWord(QString mainWord, QString assistWord, QNetworkRequest &request, QString currentIp)
{
    qDebug() << this->cookieJar->getCookies();
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
//    reply->waitForReadyRead(3000);
//    this->sleep(3);
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
            this->updateCookie();
        }
        else
        {
            resultCode = 1;
        }
    } else {
        if (this->networkManager->networkAccessible() != QNetworkAccessManager::Accessible) resultCode = 2;
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
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Referer", refer.toLocal8Bit());
    request.setRawHeader("Connection", "keep-alive");
}

QString QueryThread::buildUrl(QString mainWord, QString assistWord)
{
    int randTs = CommonUtils::rand(1223145) + 100000;
    QString url = QString("http://m.baidu.com/s?word=%1&sa=tb&ts=%2&rq=%3").arg(mainWord).arg(randTs).arg(assistWord);
    qDebug() << "get url :" << url;
    return url;
}
/**
  * update cookie for BIDUPSID
  */
void QueryThread::updateCookie()
{
    QList<QNetworkCookie> cookies = this->cookieJar->getCookies();
    if(cookies.isEmpty()) return;
    QString baiduid = "";
    QString bidupsid = "";
    QDateTime expireDate;
    foreach(QNetworkCookie cookie, cookies)
    {
        if (cookie.name() == "BAIDUID") {
            baiduid = cookie.value();
            expireDate = cookie.expirationDate();
        }
        if (cookie.name() == "BIDUPSID") bidupsid = cookie.value();
    }
    if (baiduid.isEmpty()) return;
    if (bidupsid.isEmpty())
    {
        bidupsid = baiduid.split(":").first();
        QNetworkCookie cookieSet(QString("BIDUPSID").toLocal8Bit(), bidupsid.toLocal8Bit());
        cookieSet.setDomain(".baidu.com");
        cookieSet.setPath("/");
        cookieSet.setExpirationDate(expireDate);
        cookies.append(cookieSet);
        this->cookieJar->setCookies(cookies);
    }
}
