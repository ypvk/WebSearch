import urllib2
from PySide import QtGui, QtCore
from PySide.QtCore import QThread

URL_BASE = "http://m.baidu.com"
null_proxy_handler = urllib2.ProxyHandler({})

class QueryThread(QThread):
    def __init__(self, mainKeyWord, assitKeyWord, proxys):
        self.mainKeyWord = mainKeyWord
        self.assitKeyWord = assitKeyWord
        self.proxys = proxys
        self.cookie = cookielib.CookieJar()
        self.proxyHandler = null_proxy_handler
        self.cookieHandler = urllib2.HTTPCookieProcessor(cookie)

    def run(self):
        for ip, port in proxys:
            proxy = "http://%s:%d" % (ip, port)
            self.cookie.clear()
            for i in range(20):
                 

    def buildUrl(self, word1, word2):
        url =  "%s/s?word=%s&sa=tb&ts=%d&rq=%s" % (URL_BASE, word1, 102345, word2)
        return url

    def request(self, url, proxy):
        self.proxyHandler = urllib2.ProxyHandler(proxy)
        opener = urllib2.build_opener(self.proxyHandler, self.cookieHandler)
        request = urllib2.Request(url)
        request.add_header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")
        request.add_header("User-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1")
        response = opener.open(request)
        print response.read()
