import logging
import logging.handlers

LOG_FILE="websearch.log"

handler = logging.handlers.RotatingFileHandler(LOG_FILE, maxBytes = 1024*1024, backupCount = 5)
fmt = '%(asctime)s - %(filename)s:%(lineno)s - %(name)s - %(message)s'
formatter = logging.Formatter(fmt)
handler.setFormatter(formatter) 
logger = logging.getLogger('websearch')
logger.addHandler(handler) 
logger.setLevel(logging.DEBUG)
