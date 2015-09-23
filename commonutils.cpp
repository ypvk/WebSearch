#include "commonutils.h"
#include <QtGui>

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
