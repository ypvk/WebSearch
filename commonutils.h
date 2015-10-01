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
private:
    CommonUtils();
};

#endif // COMMONUTILS_H
