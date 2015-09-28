#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>

class QWebView;
class QLineEdit;
class QUrl;
class QMutex;
class NetWorkCookieJar;
class UpdateInfo;

class Browser : public QMainWindow
{
    Q_OBJECT
public:
    explicit Browser(QWidget *parent = 0);
    void search(const QList<QString>& engines, const QList<QString> & urls,
                const QList<QString>& keyWords);
    ~Browser();
signals:
     void searchFinished();
     void updateClickInfo(const UpdateInfo& updateInfo);
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
     void rotateImages(bool invert);
     void removeGifImages();
     void removeInlineFrames();
     void removeObjectElements();
     void removeEmbeddedElements();
     void openLink(const QUrl& url);
     void loadUrl(const QUrl& url);
     void onTabTimeOut();
     void onLinkTimeOut();
     void startSearch();
     void startSearchForBaidu();
     void startSearchForMBaidu();
     void onSearchFinished();
private:
     void hrefClick();
     void mHrefClick();
     void buttonClick(const QPoint& pos);
     void clearCookie();
     void baseHrefClick(const QString& lickItemSelector);

 private:
     QString jQuery;
     QWebView *view;
     QLineEdit *locationEdit;
     QLineEdit *keyWordEdit;
     QAction *rotateAction;
     QTabWidget* tabwidget;
     int progress;
     QMutex* mutex;
     QList<int>* list;
     bool shouldBack;
     NetWorkCookieJar* cookieJar;
     //value
     QList<QString> engines;
     QList<QString> urls;
     QList<QString> keyWords;
     //current
     QString currentEngine;
     QString currentUrl;
     QString currentKeyWord;
     QString currentLinkName;
     QString currentLinkUrl;

     bool isStop;
};

#endif // BROWSER_H
