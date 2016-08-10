import os
import sys
import urllib
import urllib2
from multiprocessing import Process

outRoot = sys.argv[1]

user_agent = 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
headers = { 'User-Agent' : user_agent }
def getImageAndSave(urlRoot, fileName, outRoot):
    try:
        request = urllib2.Request(url, headers = headers)
        response = urllib2.urlopen(request)

        f = open(os.path.join(outRoot, fileName), 'wb+')
        f.write(response.read())
        f.close()
    except urllib2.URLError, e:
        pass

def
