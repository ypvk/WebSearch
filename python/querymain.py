#-*- coding:utf-8 -*-
import urllib2
import cookielib
import random
import time
#from PySide import QtGui, QtCore
#from PySide.QtCore import QThread
import threading

URL_BASE = "http://m.baidu.com"
HOST = "m.baidu.com"
null_proxy_handler = urllib2.ProxyHandler({})
PROXY_URL="http://vxer.daili666api.com/ip/?tid=555611790381221&num=800&delay=1"

class QueryThread(threading.Thread):
	"""
	query thread
	"""
	def __init__(self, index, mainKeyWord, assitKeyWord, proxy):
		threading.Thread.__init__(self)
		self.index = index
		self.mainKeyWord = mainKeyWord
		self.assitKeyWord = assitKeyWord
		self.proxy = proxy
		self.cookie = cookielib.CookieJar()
		self.proxyHandler = null_proxy_handler
		self.cookieHandler = urllib2.HTTPCookieProcessor(self.cookie)
		self.lastUrl = ""

	def run(self):
		proxy = ""
		if self.proxy:
			proxy = "http://%s" % self.proxy.strip()
		self.cookie.clear()
		for i in range(2):
			time.sleep(3)
			url = self.buildUrl(self.mainKeyWord, self.assitKeyWord)
			try:
				self.request(url, proxy)
			except Exception, e:
				print e
				break
			self.lastUrl = url
			time.sleep(3)
			url = self.buildUrl(self.assitKeyWord, self.mainKeyWord)
			try:
				self.request(url, proxy)
			except Exception, e:
				print e
				break
			self.lastUrl = url

	def buildUrl(self, word1, word2):
		random_ts = random.randint(100000, 150000);	
		url =  "%s/s?word=%s&sa=tb&ts=%d&rq=%s" % (URL_BASE, word1, 102345, word2)
		return url

	def request(self, url, proxy):
		if proxy:
			self.proxyHandler = urllib2.ProxyHandler({"http":proxy})
		opener = urllib2.build_opener(self.proxyHandler, self.cookieHandler)
		request = urllib2.Request(url)
		request.add_header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")
		request.add_header("User-agent", "MQQBrowser/26 Mozilla/5.0 (Linux; U; Android 2.3.7; zh-cn; MB200 Build/GRJ22; CyanogenMod-7) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1")
		request.add_header("Referer", self.lastUrl)
		request.add_header("Host", HOST)
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

def query_main(threadnum, times, mainKeyWord, assitKeyWord):
    """
    query main
    """
    threads = []
    for i in range(times):
        try:
            total_proxys = get_proxys(PROXY_URL)
        except Exception, e:
          print e
          time.sleep(60)
          continue
        j = 0
        while len(total_proxys) > 0: 
            proxy = total_proxys.pop()
            thread = QueryThread(j, mainKeyWord, assitKeyWord, proxy)
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
        time.sleep(180)
