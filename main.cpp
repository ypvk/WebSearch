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

//#define FIRST_INIT
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
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream.setCodec(QTextCodec::codecForName("System"));
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}
int main(int argc, char *argv[])
{
    //for qsettings
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("WebSearch");
    //for qDebug
    qInstallMsgHandler(customMessageHandler);
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
    return a.exec();
}

//void sleep1(long time)
//{
//    QEventLoop loop;
//    QTimer timer;
//    timer.setSingleShot(true);
//    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
//    timer.start(time);
//    loop.exec();
//    timer.stop();
//}
