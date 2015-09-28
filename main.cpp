#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QDebug>
#include "mainwindow.h"
#include "browser.h"
#include "dbutil.h"

//#define FIRST_INIT

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //init codec
    QTextCodec *utg8TC = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForTr(utg8TC);
    QTextCodec::setCodecForLocale(utg8TC);
    QTextCodec::setCodecForCStrings(utg8TC);

#ifdef FIRST_INIT
    qDebug() << "============first init=============";
    DBUtil::clearTables();
    DBUtil::initTables();
    DBUtil::initValues();
    DBUtil::test();
    qDebug() << "============end====================";
#endif

    MainWindow w;
    w.show();
//    Browser w;
//    w.show();
//    QList<QString> urls = QList<QString>();
//    QList<QString> keyWords = QList<QString>();
////    for (int i = 0; i< 2; i++) {
////        urls.append("http://www.baidu.com");
////        keyWords.append("musper");
////    }
//    for (int i = 0; i < 20; i++) {
//        urls.append("http://m.baidu.com");
//        keyWords.append("musper");
//    }
//    w.search(urls, keyWords);
    return a.exec();
}
