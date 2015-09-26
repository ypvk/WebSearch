#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QtWebKit/QWebPage>

class WebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit WebPage(QWidget *parent = 0);

signals:
    void loadLink(const QUrl & url);
    void openLink(const QUrl & url);
protected:
    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type);
};

#endif // WEBPAGE_H