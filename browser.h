#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>
#include <QMap>
#include "engineconfig.h"
#include "clickinfo.h"

class QWebView;
class QLineEdit;
class QUrl;
class QMutex;
class NetWorkCookieJar;
class UpdateInfo;
class QTimer;
//class ClickInfo;

class Browser : public QMainWindow
{
    Q_OBJECT
public:
    explicit Browser(QWidget *parent = 0);
    explicit Browser(QWidget* parent = 0, int id = 0);
    void search(const QList<ClickInfo>& clickInfos);
    ~Browser();
signals:
     void searchFinished();
     void updateClickInfo(const UpdateInfo& updateInfo);
     void jobFinished(int id);
public slots:
     void stopSearch();
protected slots:
     void adjustLocation();
     void changeLocation();
     void adjustTitle();
     void setProgress(int p);
     void finishLoading(bool);

     void viewSource();
     void slotSourceDownloaded();

     void highlightAllLinks();
     void openLink(const QUrl& url);
     void loadUrl(const QUrl& url);
     void onTabTimeOut();
     void onLinkTimeOut();
     void startSearch();
     void startFillKeyWord();
     void startSubmit();
     void startHrefClick();
     void onSearchFinished();
     void checkIfLoadFinished();
private:
     void init();
     void buttonClick(const QPoint& pos);
     void clearCookie();
     void baseHrefClick(const QString& lickItemSelector);
     void checkAndEmitRealtimeInfo();
     void initConfig();

 private:
     QString jQuery;
     QWebView *view;
     QLineEdit *locationEdit;
     QLineEdit *keyWordEdit;
     QTabWidget* tabwidget;
     int progress;
     QMutex* mutex;
     QList<int>* list;
     bool shouldBack;
     NetWorkCookieJar* cookieJar;
     //value
     QList<ClickInfo> clickInfos;
     //current
//     ClickInfo currentClickInfo;
     int currentWordIndex;
     int currentClickNum;
     QString currentEngine;
     QString currentUrl;
     QString currentKeyWord;
     QString currentLinkName;
     QString currentLinkUrl;
     QString currentIp;

     bool isStop;
     bool searchFlag;
     //engine config map
     QMap<QString, EngineConfig> engineConfigMap;
     QString searchEngineKey;

     QTimer* timer;

     //id
     int id;
};

#endif // BROWSER_H
