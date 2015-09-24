#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>

class QWebView;
class QLineEdit;
class QUrl;
class QMutex;

class Browser : public QMainWindow
{
    Q_OBJECT
public:
    explicit Browser(QWidget *parent = 0);
    ~Browser();

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
     void onTimeOut();
     void startSearch();
private:
     void hrefClick();

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

};

#endif // BROWSER_H
