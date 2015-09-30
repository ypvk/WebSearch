#ifndef ENGINEINFO_H
#define ENGINEINFO_H
#include <QString>

class EngineInfo
{
public:
    EngineInfo(const QString& engineName, const QString& engineUrl);
    EngineInfo(const EngineInfo& other);
    EngineInfo();
    QString getEngineName();
    QString getEngineUrl();
private:
    QString engineName;
    QString engineUrl;
};

#endif // ENGINEINFO_H
