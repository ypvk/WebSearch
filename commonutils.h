#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QString>

class CommonUtils
{
public:
    static void sleep(long time);
    static int rand(long n);
    static QString getMyIp();
private:
    CommonUtils();
};

#endif // COMMONUTILS_H
