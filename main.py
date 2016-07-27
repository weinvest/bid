import Image
import os
import sys
import shutil

R = 0
G = 1
B = 2
H = 0
S = 1
V = 2

WHITE_COLOR = (255, 255, 255)
BLACK_COLOR = (0, 0, 0)
def Convert2HSV(c):
    maxC = max(c)
    minC = min(c)
    if maxC == minC:
        return (0, 0, maxC)

    rng = float(maxC - minC)
    h = 0
    if c[R] == maxC:
        h = (c[G] - c[B]) / rng
    elif c[G] == maxC:
        h = 2 + (c[B] - c[R]) / rng
    else:
        h = 4 + (c[R] - c[G]) / rng

    h *= 60
    if h < 0:
        h += 360

    return (h, rng * 255.0 / maxC, maxC)

BACKGROUND_MIN_THRESHOLD = 3
BACKGROUND_MAX_THRESHOLD = 252
def IsBackground(c):
    if c[R] < BACKGROUND_MIN_THRESHOLD and c[G] < BACKGROUND_MIN_THRESHOLD and c[B] < BACKGROUND_MIN_THRESHOLD:
        return True

    if c[R] > BACKGROUND_MAX_THRESHOLD and c[G] > BACKGROUND_MAX_THRESHOLD and c[B] > BACKGROUND_MAX_THRESHOLD:
        return True

    return False

SAME_PIXEL_H_THRESOLD = 19
SAME_PIXEL_SV_THRESOLD = 25000
def IsSamePixel(c1, c2):
    if IsBackground(c1) or IsBackground(c2):
        return False

    hsv1 = Convert2HSV(c1)
    hsv2 = Convert2HSV(c2)
    hDiff = abs(hsv1[H] - hsv2[H])
    if hDiff < SAME_PIXEL_H_THRESOLD or hDiff > (360 - SAME_PIXEL_H_THRESOLD):
        sDiff = hsv1[S] - hsv2[S]
        vDiff = hsv1[V] - hsv2[V]
        if sDiff * sDiff + vDiff * vDiff < SAME_PIXEL_SV_THRESOLD:
            return True

    return False

def ToHSVList(bmp, l, t, r, b):
    hsvs = []
    for h in xrange(t, b):
        for w in xrange(l, r):
            hsvs.append(Convert2HSV(bmp.getpixel((w, h))))
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
        if IsSamePixel(c, cc):
            #print '(%d,%d) c=(%d,%d,%d) cc=(%d,%d,%d)' % (w,h, c[0], c[1], c[2], cc[0], cc[1], cc[2])
            origin.putpixel((w + ww, h + hh), WHITE_COLOR)
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
            if not IsBackground(c):
                if create:
                    childBmps.append(None)

                childBmps[-1] = Image.new('RGB', (bmp.width, bmp.height))
                me = childBmps[-1]

                me.putpixel((w, h), c)
                bmp.putpixel((w, h), WHITE_COLOR)
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
            SimlarAndRemoveLine(bmp, fileName)

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
