#include "commonutils.h"
#include <QtGui>
#include <ctime>
#include <QtNetwork>

CommonUtils::CommonUtils()
{
}

void CommonUtils::sleep(long time)
{
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(time);
    loop.exec();
    timer.stop();
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

QMap<QString, EngineConfig> CommonUtils::getEngineConfigs()
{
    QMap<QString, EngineConfig> engineConfigMap;
    QSettings *configIniRead = new QSettings("config.ini", QSettings::IniFormat);
    QStringList engineGroups = configIniRead->childGroups();
    for(int i = 0; i < engineGroups.size(); i++) {
        QString hostNameKey = engineGroups.at(i) + "/hostname";
        QString inputTextKey = engineGroups.at(i) + "/input_text";
        QString inputSubmitKey = engineGroups.at(i) + "/input_submit";
        QString hreflinkKey = engineGroups.at(i) + "/href";
        QString hostName = configIniRead->value(hostNameKey).toString();
        QString inputText = configIniRead->value(inputTextKey).toString();
        QString inputSubmit = configIniRead->value(inputSubmitKey).toString();
        QString hrefLink = configIniRead->value(hreflinkKey).toString();
        engineConfigMap[hostName] = EngineConfig(hostName, inputText, inputSubmit, hrefLink);
    }
    delete configIniRead;
    return engineConfigMap;
}
