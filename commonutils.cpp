#include "commonutils.h"
#include <QtGui>
#include <ctime>
#include <QtNetwork>

CommonUtils::CommonUtils()
{
}

void CommonUtils::sleep(long time)
{
    QTime t;
    t.start();
    while(t.elapsed()<2000) {
        QCoreApplication::processEvents();
        usleep(10000);
    }
}

int CommonUtils::rand(long n)
{
    qsrand(time(NULL));
    return qrand() % n;
}

QString CommonUtils::getMyIp()
{
    QString myipv4Address;
#ifdef _WIN32
    QHostInfo vHostInfo = QHostInfo::fromName(QHostInfo::localHostName());
    QList<QHostAddress> vAddressList = vHostInfo.addresses();
#else
    QList<QHostAddress> vAddressList = QNetworkInterface::allAddresses();
#endif
    for(int i = 0; i < vAddressList.size(); i++)
    {
        if(!vAddressList.at(i).isNull() &&
           vAddressList.at(i) != QHostAddress::LocalHost &&
           vAddressList.at(i).protocol() ==  QAbstractSocket::IPv4Protocol)
        {
            myipv4Address = vAddressList.at(i).toString();
            break;
        }
    }
    return myipv4Address;
}
