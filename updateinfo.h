#ifndef UPDATEINFO_H
#define UPDATEINFO_H

#include <QString>

class UpdateInfo
{
public:
    UpdateInfo(const QString& engineName, const QString& engineUrl,
               const QString& keyWord, const QString& clickName,
               const QString& clickUrl, const QString& ip);
public:
    QString engineName;
    QString engineUrl;
    QString keyWord;
    QString clickName;
    QString clickUrl;
    QString ip;
};

#endif // UPDATEINFO_H
