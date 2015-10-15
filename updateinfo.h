#ifndef UPDATEINFO_H
#define UPDATEINFO_H

#include <QString>
#include <QPair>

class UpdateInfo
{
public:
    UpdateInfo(const QString& engineName, const QString& engineUrl,
               const QPair<QString, QString>& keyWord, const QString& clickName,
               const QString& clickUrl, const QString& ip);
public:
    QString engineName;
    QString engineUrl;
    QPair<QString, QString> keyWord;
    QString clickName;
    QString clickUrl;
    QString ip;
};

#endif // UPDATEINFO_H
