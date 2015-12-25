#-*- coding:utf-8 -*-
import urllib2
import urllib
import cookielib
import random
import time
#from PySide import QtGui, QtCore
#from PySide.QtCore import QThread
import threading
from logger import logger

URL_BASE = "http://m.baidu.com"
HOST = "m.baidu.com"
null_proxy_handler = urllib2.ProxyHandler({})
PROXY_URL="http://vxer.daili666api.com/ip/?tid=555611790381221&num=800&delay=1"

class QueryThread(threading.Thread):
    """
    query thread
    """
    def __init__(self, index, mainKeyWord, assitKeyWords, proxy):
        threading.Thread.__init__(self)
        self.index = index
        self.mainKeyWord = mainKeyWord
        self.assitKeyWords = assitKeyWords
        self.proxy = proxy
        self.cookie = cookielib.CookieJar()
        self.proxyHandler = null_proxy_handler
        self.cookieHandler = urllib2.HTTPCookieProcessor(self.cookie)
        self.lastUrl = ""
        self.step = self.index % 5  # step is 0 search 2 times stop is 1 search 3 times
        if self.step > 0:
            self.step = 1

    def run(self):
        proxy = ""
        if self.proxy:
            proxy = "http://%s" % self.proxy.strip()
        self.cookie.clear()
        for i in range(len(self.assitKeyWords)):
            result = True
            self.assitKeyWord = self.assitKeyWords[i]
            if self.step == 0:
                result = self.searchStep1(proxy)
            elif self.step == 1:
                result = self.searchStep(proxy)
            if not result:
                break

    def searchStep(self, proxy):
        url = URL_BASE
        try:
            self.request(url, proxy)
        except Exception, e:
            print e
            return False
        self.lastUrl = url
        time.sleep(5)
        url = self.buildUrl(self.mainKeyWord, "", 'ib')
        try:
            self.request(url, proxy)
        except Exception, e:
            print e
            return False
        self.lastUrl = url
        time.sleep(5)
        url = self.buildUrl(self.assitKeyWord, self.mainKeyWord, 'tb')
        try:
            self.request(url, proxy)
        except Exception, e:
            print e
            return False
        time.sleep(5)
        return True

    def searchStep1(self, proxy):
        url = URL_BASE
        try:
            self.request(url, proxy)
        except Exception, e:
            print e
            return False
        self.lastUrl = url
        time.sleep(5)
        url = self.buildUrl(self.assitKeyWord, "", 'ib')
        try:
            self.request(url, proxy)
        except Exception, e:
            print e
            return False
        time.sleep(5)
        return True

    def buildUrl(self, word1, word2, sa):
        random_ts = random.randint(1500000, 2000000)
        params = {}
        if sa == "ib":
            params = {"word": word1, "sa": "ib", "ts": random_ts}
            if word2:
                params['rq'] = word2
        else:
            params = {"word": word1, "sa": "tb"}
            if word2:
                params['rq'] = word2
        url = "%s/s?%s" % (URL_BASE, urllib.urlencode(params))
        return url

    def request(self, url, proxy):
        if proxy:
            self.proxyHandler = urllib2.ProxyHandler({"http":proxy})
        opener = urllib2.build_opener(self.proxyHandler, self.cookieHandler)
        request = urllib2.Request(url)
        request.add_header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")
        request.add_header("User-agent", "MQQBrowser/26 Mozilla/5.0 (Linux; U; Android 2.3.7; zh-cn; MB200 Build/GRJ22; CyanogenMod-7) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1")
        request.add_header("Accept-Encoding", "deflate")
        request.add_header("Accept-Language", "en-us,en;q=0.5")
        request.add_header("Connection", "keep-alive")
        request.add_header("Referer", self.lastUrl)
        request.add_header("Host", HOST)
        #print request.headers
        response = opener.open(request, timeout=3)
        print "thread: %d url:%s proxy:%s" % (self.index, url, proxy) 
        print self.cookie
        print response.read()[:500].decode("utf-8")

def get_proxys(url):
	"""
	get proxys
	"""
	response = urllib2.urlopen(url, timeout=10)
	data = response.readlines()
	return data

def query_main(threadnum, times, mainKeyWord, assitKeyWords):
    """
    query main
    """
    threads = []
    i = 0
    while i < times:
        logger.info("query times:%d" % i)
        try:
            total_proxys = get_proxys(PROXY_URL)
        except Exception, e:
            print e
            time.sleep(60)
            continue
        j = 0
        while len(total_proxys) > 0: 
            proxy = total_proxys.pop()
            thread = QueryThread(j, mainKeyWord, assitKeyWords, proxy)
            thread.setDaemon(True)
            thread.start()
            j = j + 1
            threads.append(thread)
            if j == threadnum:
                for thread in threads:
                    thread.join()
                threads = []
                j = 0
        #sleep 3 min
        #time.sleep(180)
        i = i + 1
