#!/usr/bin/env python
import sys
import os
import Image
import InterferingLine
from recognize import Regonizer
from segment import SimlarAndRemoveLine
def ToHSVList(bmp, l, t, r, b):
    hsvs = []
    for h in xrange(t, b):
        for w in xrange(l, r):
            hsvs.append(rgb2hsv(bmp.getpixel((w, h))))
    return hsvs

if __name__ == '__main__':
    reg = Regonizer('./vcfonts')
    # m = reg.computeSelfSimilarMatrix()
    # m.to_csv('similarMatrix.csv', float_format='%.2f')

    bmpRoot = sys.argv[1]
    for fileName in os.listdir(bmpRoot):
        [stem, ext] = os.path.splitext(fileName)
        if '.bmp' == ext:
            bmpPath = os.path.join(bmpRoot, fileName)
            bmp = Image.open(bmpPath)
            InterferingLine.clean(bmp)
            childBmps, binaryBmps = SimlarAndRemoveLine(bmp, fileName)
            values = []
            globalSimilar = 1.0
            for binaryBmp in binaryBmps:
                value, similar = reg.regonizeSlow(binaryBmp.reshape((1, binaryBmp.size)))
                if value is not None and similar > 0.75:
                    values.append(value)
                    globalSimilar *= similar
            print('%s:%f:%s' % (fileName, globalSimilar, str(values)))

# bmpPath = sys.argv[1]
# bmp = Image.open(bmpPath)
# rng = [int(i) for i in sys.argv[2].split(',')]
# hsvs = ToHSVList(bmp, rng[0], rng[1], rng[2], rng[3])
#
# import pandas as pd
# dHSV = pd.DataFrame(hsvs, columns = list('hsv'))
# dHSV = dHSV[dHSV.h != 0]
# dHSV['h'].hist(bins = 72)
#
# import matplotlib.pyplot as pl
# pl.show()
