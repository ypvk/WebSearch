#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>

class QWebView;
class QLineEdit;

class Browser : public QMainWindow
{
    Q_OBJECT
public:
    explicit Browser(QWidget *parent = 0);

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

 private:
     QString jQuery;
     QWebView *view;
     QLineEdit *locationEdit;
     QAction *rotateAction;
     int progress;

};

#endif // BROWSER_H
