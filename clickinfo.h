#ifndef CLICKINFO_H
#define CLICKINFO_H

#include "engineinfo.h";
#include <QList>

class EngineInfo;

class ClickInfo
{
public:
    ClickInfo();
    ClickInfo(const EngineInfo& engineInfo, const QList<QString>& keyWords, int clickNum);
    ClickInfo(const ClickInfo& other);
    ClickInfo& operator=(const ClickInfo& other);
    ~ClickInfo();
    EngineInfo& getEngineInfo();
    QList<QString>& getKeyWords();
    int getClickNum();
    void setClickNum(int clickNum);
private:
    EngineInfo engineInfo;
    QList<QString> keyWords;
    int clickNum;
};

#endif // CLICKINFO_H
