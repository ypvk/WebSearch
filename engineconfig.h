#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include <QString>

class EngineConfig
{
public:
    EngineConfig(const QString& hostUrl,
                 const QString& inputText,
                 const QString& inputSubmit,
                 const QString& hrefLink);
    EngineConfig();
public:
    QString hostUrl;
    QString inputText;
    QString inputSubmit;
    QString hrefLink;
};

#endif // ENGINECONFIG_H
