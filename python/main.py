#-*- coding:utf-8 -*-
import sys
from querymain import query_main
from logger import logger

def main(argv):
    logger.info("start")
    assitWords = ["musper真搞笑", "musper真好玩", "musper好厉害", "musper挺仗义"]
    query_main(20, 1000000, "musper", assitWords)
if __name__ == "__main__":
    main(sys.argv)
