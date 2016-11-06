#!/usr/bin/env python
import sys
import os
import Image
import numpy
import color
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
    f = f.replace('Y','')
    f = f.replace('C', '')
    f = f.replace('MEN', '')
    f = f.replace('BAN', '')
    f = f.replace('APP', '')
    f = f.replace('PT', '')
    return (f, f[4:] + f[0:4])

def SelectAndSaveElement(bmp, fileName, outRoot, ext):
    outDir = os.path.join(outRoot, fileName)
    if os.path.isdir(outDir):
        import shutil
        shutil.rmtree(outDir)
    os.mkdir(outDir)

    childBmps, binaryBmps = SimlarAndRemoveLine(bmp, fileName, 0.6)
    count = len(childBmps) - 1
    while count >= 0:
        eleBmp = childBmps[count]
        fmt = ext[1:].upper()
        eleBmp.save(os.path.join(outDir, str(count) + ext), format='BMP')
        binBmp = binaryBmps[count]
        import numpy as np
        np.savetxt(os.path.join(outDir, str(count) + '.txt'), binBmp, fmt='%1.0f')
        count -= 1

def emplaceBits(bmp):
    result = numpy.zeros((bmp.height, bmp.width), int)
    for h in range(1, bmp.height - 1):
        for w in range(1, bmp.width - 1):
            c = bmp.getpixel((w, h))
            if not color.isBackground(c):
                def getValue(ww, hh):
                    return 0 if color.isBackground(bmp.getpixel((ww, hh))) else 1
                v = getValue(w-1, h-1) + getValue(w-1, h) + getValue(w-1, h+1) + getValue(w, h-1) + getValue(w,h+1)
                v += getValue(w+1, h-1) + getValue(w+1, h) + getValue(w+1, h+1)
                result[h, w] = v

    return result


if __name__ == '__main__':
    reg = FullRecognizer('./vcfonts')
    # m = reg.computeSelfSimilarMatrix()
    # m.to_csv('similarMatrix.csv', float_format='%.2f')

    bmpRoot = sys.argv[1]
    success = []
    failed = []
    print bmpRoot
    for fileName in os.listdir(bmpRoot):
        [stem, ext] = os.path.splitext(fileName)
        # print stem,ext
        if '.bmp' == ext or '.png' == ext or '.jpg' == ext:
            relValue, relValue2 = getRealValue(stem)
            bmpPath = os.path.join(bmpRoot, fileName)
            bmp = Image.open(bmpPath)
            result1 = emplaceBits(bmp)
            numpy.savetxt('/tmp/result/' + fileName + '.1', result1, fmt="%d", delimiter='')
            result1[result1<=3] = 0
            result1[result1>0] = 1
            numpy.savetxt('/tmp/result/' + fileName+'.2', result1, fmt="%d", delimiter='')
            #
            # import segment
            # import numpy as np
            # index = 0
            # for binaryImg in segment.Segment(bmp):
            #    np.savetxt(os.path.join('/tmp/', str(index) + '.txt'), binaryImg, fmt='%1.0f')
            #    index += 1
            #
            # SelectAndSaveElement(bmp, fileName, '/tmp/', '.bmp')
            #InterferingLine.clean(bmp)
            print fileName
            # values = reg.regonize(bmp, (20, 2), (90, 45))

            # values = reg.regonizeEx(bmp)
            # v = ''.join(values)
            # if v != relValue and v != relValue2:
            #     failed.append((fileName, v))
            # else:
            #     success.append(v)

    print 'success:%d' % len(success)
    print success
    print 'failed:%d' % len(failed)
    print failed
