#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include "mainwindow.h"
#include "browser.h"
#include "dbutil.h"
#include "commonutils.h"

//#define FIRST_INIT

int main(int argc, char *argv[])
{
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
