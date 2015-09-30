#include "engineinfo.h"

EngineInfo::EngineInfo()
{

}

EngineInfo::EngineInfo(const QString &engineName, const QString &engineUrl):engineName(engineName), engineUrl(engineUrl)
{
}

EngineInfo::EngineInfo(const EngineInfo &other)
{
    this->engineName = other.engineName;
    this->engineUrl = other.engineUrl;
}

QString EngineInfo::getEngineName()
{
    return engineName;
}
QString EngineInfo::getEngineUrl()
{
    return engineUrl;
}
