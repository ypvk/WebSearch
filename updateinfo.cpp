#include "updateinfo.h"
#include <QString>

UpdateInfo::UpdateInfo(QObject *parent) : QObject(parent)
{

}

UpdateInfo::UpdateInfo(const QString& engineName, const QString& engineUrl,
                       const QPair<QString, QString>& keyWord, const QString& clickName,
                       const QString& clickUrl, const QString& ip)
{
    this->engineName = engineName;
    this->engineUrl = engineUrl;
    this->keyWord = keyWord;
    this->clickName = clickName;
    this->clickUrl = clickUrl;
    this->ip = ip;
}
