#!/usr/bin/env python
import sys
import os
import Image
import InterferingLine
from recognize import Regonizer
from fullRecognize import FullRecognizer
from segment import SimlarAndRemoveLine
def ToHSVList(bmp, l, t, r, b):
    hsvs = []
    for h in xrange(t, b):
        for w in xrange(l, r):
            hsvs.append(rgb2hsv(bmp.getpixel((w, h))))
    return hsvs

def getRealValue(f):
    f = f.replace('green', '')
    f = f.replace('men', '')
    f = f.replace('red', '')
    f = f.replace('ban', '')
    f = f.replace('app', '')
    f = f.replace('pt', '')
    return (f, f[4:] + f[0:4])

def SelectAndSaveElement(bmp, fileName, outRoot, ext):
    outDir = os.path.join(outRoot, fileName)
    if os.path.isdir(outDir):
        import shutil
        shutil.rmtree(outDir)
    os.mkdir(outDir)

    childBmps, binaryBmps = SimlarAndRemoveLine(bmp, fileName, 0.0)
    count = len(childBmps) - 1
    while count >= 0:
        eleBmp = childBmps[count]
        fmt = ext[1:].upper()
        eleBmp.save(os.path.join(outDir, str(count) + ext), format='BMP')
        binBmp = binaryBmps[count]
        import numpy as np
        np.savetxt(os.path.join(outDir, str(count) + '.txt'), binBmp, fmt='%1.0f')
        count -= 1

if __name__ == '__main__':
    reg = FullRecognizer('./vcfonts')
    # m = reg.computeSelfSimilarMatrix()
    # m.to_csv('similarMatrix.csv', float_format='%.2f')

    bmpRoot = sys.argv[1]
    success = []
    failed = []
    for fileName in os.listdir(bmpRoot):
        [stem, ext] = os.path.splitext(fileName)
        if '.bmp' == ext or '.png' == ext or '.jpg' == ext:
            relValue, relValue2 = getRealValue(stem)
            bmpPath = os.path.join(bmpRoot, fileName)
            bmp = Image.open(bmpPath)
            # SelectAndSaveElement(bmp, fileName, '/tmp/', '.bmp')
            InterferingLine.clean(bmp)
            values = reg.regonize(bmp, (20, 2), (90, 45))
            v = ''.join(values)
            if v != relValue and v != relValue2:
                failed.append((fileName, v))
            else:
                success.append(v)

    print 'success:'
    print success
    print 'failed:'
    print failed
            # childBmps, binaryBmps = SimlarAndRemoveLine(bmp, fileName, 0.1)
            # values = []
            # globalSimilar = 1.0
            # for binaryBmp in binaryBmps:
            #     value, similar = reg.regonizeSlow(binaryBmp.reshape((1, binaryBmp.size)))
            #     if value is not None and similar > 0.75:
            #         values.append(value)
            #         globalSimilar *= similar
            # print('%s:%f:%s' % (fileName, globalSimilar, str(values)))

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
