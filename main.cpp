#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include "mainwindow.h"
#include "browser.h"
#include "dbutil.h"
#include "commonutils.h"
#include <QtCore>
#include <QtGui>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "networkcookiejar.h"

//#define FIRST_INIT

void customMessageHandler(QtMsgType type, const char *msg);
void testGetRequest();

int main(int argc, char *argv[])
{
    //for qsettings
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("WebSearch");
    //for qDebug
  //  qInstallMsgHandler(customMessageHandler);
    QApplication a(argc, argv);
    //init codec
    QTextCodec *utg8TC = QTextCodec::codecForName("utf-8");
//    QTextCodec *utg8TC = QTextCodec::codecForName("System");
    QTextCodec::setCodecForTr(utg8TC);
    QTextCodec::setCodecForLocale(utg8TC);
    QTextCodec::setCodecForCStrings(utg8TC);

#ifdef FIRST_INIT
    qDebug() << "============first init=============";
    DBUtil::clearTables();
    DBUtil::initTables();
//    DBUtil::initValues();
    DBUtil::test();
    qDebug() << "============end====================";
#endif
    MainWindow w;
    w.show();
    testGetRequest();
    return a.exec();
}

void testGetRequest()
{
    QString url = "http://m.baidu.com/s?word=abc&ts=1223145&rq=ab";
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    //request.setRawHeader("Accept-Encoding", "gzip, deflate, sdch");
    request.setRawHeader("Accept-Language", "h-CN,zh;q=0.8");
    request.setRawHeader("Host", "m.baidu.com");
    request.setRawHeader("Referer", "http://m.baidu.com");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.93 Safari/537.36");
    QNetworkReply* reply = networkAccessManager->get(request);
    QEventLoop loop;
    NetWorkCookieJar* cookieJar = new NetWorkCookieJar(networkAccessManager);
    networkAccessManager->setCookieJar(cookieJar);
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(5000);
    loop.exec();
    timer.stop();
    qDebug() << request.rawHeaderList();
    qDebug() << reply->readAll();
    qDebug() << cookieJar->getCookies();
    networkAccessManager->deleteLater();
    reply->deleteLater();
}

void customMessageHandler(QtMsgType type, const char *msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3").arg(text).arg(current_date).arg(msg);

    QFile file("log.txt");
    if (file.size() > 104857600)//size large than 100M split
    {
        QString filedate = QDateTime::currentDateTime().toString("yyyy-MM-dd--hh-mm-ss");
        file.rename(QString("log_%1.txt").arg(filedate));
        file.setFileName("log.txt");
    }
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream.setCodec(QTextCodec::codecForName("System"));
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

