#-*- coding:utf-8 -*-
import sys
from querymain import query_main
from logger import logger

def main(argv):
	logger.info("start")
	query_main(1, 1000000, "musper", "musper真好玩")
if __name__ == "__main__":
    main(sys.argv)
