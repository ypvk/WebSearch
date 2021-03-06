#ifndef CLICKINFO_H
#define CLICKINFO_H

#include "engineinfo.h";
#include <QList>
#include <QPair>

class EngineInfo;

class ClickInfo
{
public:
    ClickInfo();
    ClickInfo(const EngineInfo& engineInfo,
              const QList<QPair<QString, QString> >& keyWords,
              const QList<QPair<QString, int> >& proxys,
              int clickNum);
    ClickInfo(const ClickInfo& other);
    ClickInfo& operator=(const ClickInfo& other);
    ~ClickInfo();
    EngineInfo& getEngineInfo();
    QList<QPair<QString, QString> >& getKeyWords();
    QList<QPair<QString, int> >& getProxys();
    int getClickNum();
    void setClickNum(int clickNum);
private:
    EngineInfo engineInfo;
    QList<QPair<QString, int> > proxys;
    QList<QPair<QString, QString> > keyWords;
    int clickNum;
};

#endif // CLICKINFO_H
