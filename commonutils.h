#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QString>
#include <QMap>
#include "engineconfig.h";

class CommonUtils
{
public:
    static void sleep(long time);
    static int rand(long n);
    static QString getMyIp();
    static QMap<QString, EngineConfig> getEngineConfigs();
    static bool isClearCookie();
    static int getIpClickTimes();
    static QString getConfig(const QString& key);
private:
    CommonUtils();
};

#endif // COMMONUTILS_H
