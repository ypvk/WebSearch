#include "engineconfig.h"

EngineConfig::EngineConfig()
{

}

EngineConfig::EngineConfig(const QString& hostUrl,
                           const QString& inputText,
                           const QString& inputSubmit,
                           const QString& hrefLink)
{
    this->hostUrl = hostUrl;
    this->inputText = inputText;
    this->inputSubmit = inputSubmit;
    this->hrefLink = hrefLink;
}
