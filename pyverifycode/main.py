#!/usr/bin/env python
import Image
import os
import sys
import shutil
import color
import InterferingLine

def ToHSVList(bmp, l, t, r, b):
    hsvs = []
    for h in xrange(t, b):
        for w in xrange(l, r):
            hsvs.append(rgb2hsv(bmp.getpixel((w, h))))
    return hsvs

HMargin = 2
WMargin = 2
def SelectSameElementEx(origin, me, w, h, c):
    if w <= WMargin or w >= (origin.width - WMargin):
        return 0

    if h < HMargin or h >= (origin.height - HMargin):
        return 0

    count = [1]
    def spreadPixel(ww, hh):
        cc = origin.getpixel((w + ww, h + hh))
        if isSamePixel(c, cc):
            #print '(%d,%d) c=(%d,%d,%d) cc=(%d,%d,%d)' % (w,h, c[0], c[1], c[2], cc[0], cc[1], cc[2])
            origin.putpixel((w + ww, h + hh), color.WHITE_COLOR)
            me.putpixel((w + ww, h + hh), cc)
            count[0] += 1
            return True
        return False

    tl = spreadPixel(-1, -1)
    t = spreadPixel(0, -1)
    tr = spreadPixel(1, -1)
    l = spreadPixel(-1, 0)
    r = spreadPixel(1, 0)
    bl = spreadPixel(-1, 1)
    b = spreadPixel(0, 1)
    br = spreadPixel(1, 1)

    if tl: count[0] += SelectSameElementEx(origin, me, w - 1, h - 1, c)
    if t: count[0] += SelectSameElementEx(origin, me, w, h - 1, c)
    if tr: count[0] += SelectSameElementEx(origin, me, w + 1, h - 1, c)
    if l: count[0] += SelectSameElementEx(origin, me, w - 1, h, c)
    if r: count[0] += SelectSameElementEx(origin, me, w + 1, h, c)
    if bl: count[0] += SelectSameElementEx(origin, me, w - 1, h + 1, c)
    if b: count[0] += SelectSameElementEx(origin, me, w, h + 1, c)
    if br: count[0] += SelectSameElementEx(origin, me, w + 1, h + 1, c)

    return count[0];

def SimlarAndRemoveLine(bmp, fileName):
    if os.path.isdir(fileName):
        shutil.rmtree(fileName)

    os.mkdir(fileName)

    bmp.save(fileName + "/origin.bmp", "BMP")
    childBmps = []
    create = True
    for h in range(HMargin, bmp.height - HMargin):
        for w in range(WMargin, bmp.width - WMargin):
            c = bmp.getpixel((w, h))
            if not isBackground(c):
                if create:
                    childBmps.append(None)

                childBmps[-1] = Image.new('RGB', (bmp.width, bmp.height))
                me = childBmps[-1]

                me.putpixel((w, h), c)
                bmp.putpixel((w, h), color.WHITE_COLOR)
                count = SelectSameElementEx(bmp, me, w, h, c)
                create = count > 30

    bmp.save(fileName + "/out.bmp", "BMP")
    seq = 1
    for child in childBmps:
        child.save(fileName + "/" + str(seq) + ".bmp", "BMP")
        seq += 1

if __name__ == '__main__':
    bmpRoot = sys.argv[1]
    for fileName in os.listdir(bmpRoot):
        [stem, ext] = os.path.splitext(fileName)
        if '.bmp' == ext:
            bmpPath = os.path.join(bmpRoot, fileName)
            bmp = Image.open(bmpPath)
            bmp.save(fileName + '.org.bmp')
            InterferingLine.clean(bmp)
            bmp.save(fileName + '.out.bmp')

            # bmpPath = fileName + '.out.bmp'
            # bmp = Image.open(bmpPath)
            # SimlarAndRemoveLine(bmp, fileName)
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
