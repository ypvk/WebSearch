#include "clickinfo.h"
#include "engineinfo.h"
#include <QList>

ClickInfo::ClickInfo()
{
}

ClickInfo::ClickInfo(const ClickInfo &other)
{
    this->clickNum = other.clickNum;
    this->keyWords = other.keyWords;
    this->engineInfo = other.engineInfo;
    this->proxys = other.proxys;
}

ClickInfo::ClickInfo(const EngineInfo &engineInfo,
                     const QList<QString>& keyWords,
                     const QList<QPair<QString, int> >& proxys,
                     int clickNum)
{
    this->engineInfo = engineInfo;
    this->keyWords = keyWords;
    this->clickNum = clickNum;
    this->proxys = proxys;
}
ClickInfo& ClickInfo::operator=(const ClickInfo& other)
{
    this->clickNum = other.clickNum;
    this->keyWords = other.keyWords;
    this->engineInfo = other.engineInfo;
    this->proxys = other.proxys;
}

ClickInfo::~ClickInfo()
{

}

int ClickInfo::getClickNum()
{
    return clickNum;
}

EngineInfo& ClickInfo::getEngineInfo()
{
    return engineInfo;
}
QList<QString>& ClickInfo::getKeyWords()
{
    return keyWords;
}

QList<QPair<QString, int> >& ClickInfo::getProxys()
{
    return proxys;
}

void ClickInfo::setClickNum(int clickNum)
{
    this->clickNum = clickNum;
}
