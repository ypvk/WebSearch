#include <QtGui/QApplication>
#include "mainwindow.h"
#include "browser.h"
#include "dbutil.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBUtil::initTables();
    DBUtil::initValues();

    MainWindow w;
    w.show();
//    Browser w;
//    w.show();
//    QList<QString> urls = QList<QString>();
//    QList<QString> keyWords = QList<QString>();
//    for (int i = 0; i< 2; i++) {
//        urls.append("http://www.baidu.com");
//        keyWords.append("musper");
//    }
//    for (int i = 0; i < 2; i++) {
//        urls.append("http://m.baidu.com");
//        keyWords.append("musper");
//    }
    //w.search(urls, keyWords);
    return a.exec();
}
