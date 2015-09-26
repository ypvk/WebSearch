#include "networkcookiejar.h"

NetWorkCookieJar::NetWorkCookieJar(QObject* parent) : QNetworkCookieJar(parent)
{
}
NetWorkCookieJar::~NetWorkCookieJar()
{

}

QList<QNetworkCookie> NetWorkCookieJar::getCookies()
{
    return allCookies();
}

void NetWorkCookieJar::setCookies(const QList<QNetworkCookie>& cookieList)
{
    if(this == NULL)
        return;
    this->setAllCookies(cookieList);
}
