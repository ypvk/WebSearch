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
PROXY_URL="http://vxer.daili666api.com/ip/?tid=555611790381221&num=80&delay=1"

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
		request.add_header("User-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1")
		request.add_header("Referer", self.lastUrl)
		request.add_header("Host", HOST)
		response = opener.open(request)
		print "thread: %d url:%s proxy:%s" % (self.index, url, proxy) 
		print self.cookie
		print response.read()[:1000].decode("utf-8").encode("gbk", 'ignore')

def get_proxys(url):
	"""
	get proxys
	"""
	response = urllib2.urlopen(url)
	data = response.readlines()		
	return data

def query_main(threadnum, times, mainKeyWord, assitKeyWord):
	"""
	query main
	"""
	threads = []
	for i in range(times):
		total_proxys = get_proxys(PROXY_URL)
		for j in range(threadnum):
			if len(total_proxys) > 0: 
				proxy = total_proxys.pop()
			thread = QueryThread(j, mainKeyWord, assitKeyWord, "")
			thread.setDaemon(True)
			thread.start()
			threads.append(thread)
		for thread in threads:
			thread.join()
