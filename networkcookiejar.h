#ifndef NETWORKCOOKIEJAR_H
#define NETWORKCOOKIEJAR_H

#include <QNetworkCookieJar>

class NetWorkCookieJar : public QNetworkCookieJar
{

    Q_OBJECT

    public:
        NetWorkCookieJar(QObject *parent);
        ~NetWorkCookieJar();

        QList<QNetworkCookie> getCookies();
        void setCookies(const QList<QNetworkCookie>& cookieList);
};

#endif // NETWORKCOOKIEJAR_H
