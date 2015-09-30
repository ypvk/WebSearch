#include "commonutils.h"
#include <QtGui>
#include <ctime>

CommonUtils::CommonUtils()
{
}

void CommonUtils::sleep(long time)
{
    QTime t;
    t.start();
    while(t.elapsed()<2000)
        QCoreApplication::processEvents();
}

int CommonUtils::rand(long n) {
    qsrand(time(NULL));
    return qrand() % n;
}
