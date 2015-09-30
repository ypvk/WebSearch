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
}

ClickInfo::ClickInfo(const EngineInfo &engineInfo, const QList<QString>& keyWords, int clickNum)
{
    this->engineInfo = engineInfo;
    this->keyWords = keyWords;
    this->clickNum = clickNum;
}
ClickInfo& ClickInfo::operator=(const ClickInfo& other)
{
    this->clickNum = other.clickNum;
    this->keyWords = other.keyWords;
    this->engineInfo = other.engineInfo;
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

void ClickInfo::setClickNum(int clickNum)
{
    this->clickNum = clickNum;
}
