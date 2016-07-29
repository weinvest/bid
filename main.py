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

    return (h, rng * 256.0 / maxC, maxC)

BACKGROUND_MIN_THRESHOLD = 3
BACKGROUND_MAX_THRESHOLD = 252
BACKGROUND_H_V_THRESHOLD = 70 * 2.56
def IsBackground(c):
    if c[R] < BACKGROUND_MIN_THRESHOLD and c[G] < BACKGROUND_MIN_THRESHOLD and c[B] < BACKGROUND_MIN_THRESHOLD:
        return True

    if c[R] > BACKGROUND_MAX_THRESHOLD and c[G] > BACKGROUND_MAX_THRESHOLD and c[B] > BACKGROUND_MAX_THRESHOLD:
        return True

    hsv = Convert2HSV(c)
    return hsv[V] - hsv[S] > BACKGROUND_H_V_THRESHOLD

SAME_PIXEL_H_THRESOLD = 10
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
    #elif hDiff < 50 or hDiff > 310:
    #    sDiff = hsv1[S] - hsv2[S]
    #    vDiff = hsv1[V] - hsv2[V]
    #    if sDiff * sDiff + vDiff * vDiff < 1500:
    #        return True

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

def RemoveLine(bmp, fileName):
    if os.path.isdir(fileName):
        shutil.rmtree(fileName)

    os.mkdir(fileName)

    bmp.save(fileName + "/origin.bmp", "BMP")
    out = Image.new('RGB', (bmp.width, bmp.height))
    for h in range(HMargin, bmp.height - HMargin):
        for w in range(WMargin, bmp.width - WMargin):
            c = bmp.getpixel((w, h))
            if not IsBackground(c):
                firstCoor = None
                secondCoor = None
                count = 0
                for ww in range(-1, 2):
                    for hh in range(-1, 2):
                        if (0 != ww or 0 != hh ) and IsSamePixel(c, bmp.getpixel((w + ww, h + hh))):
                            count += 1
                            if 1 == count:
                                firstCoor = (ww, hh)
                            elif 2 == count:
                                secondCoor = (ww, hh)
                            else:
                                pass
                isLine = False
                if 2 == count:
                    if (firstCoor[0] * secondCoor[0] + firstCoor[1] * secondCoor[1]) <= 0:
                        isLine = True
                elif 1 >= count:
                    isLine = True
                if not isLine:
                    out.putpixel((w,h), c)

    out.save(fileName + "/out.bmp", "BMP")

def GetSimilarPixels(bmp, c, origin, excludes):
    similars = []
    w, h = origin
    for ww in range(-1, 2):
        for hh in range(-1, 2):
            if (ww, hh) not in excludes and IsSamePixel(c, bmp.getpixel((w + ww, h + hh))):
                similars.append((ww, hh))

    return similars

def TraverseLine(bmp, w, h, fromDirection, directions, continuesDo = None):
    def callContinus():
        if continuesDo is None:
            return True

        newDirections = []
        for direction in directions:
            newDirections.append((-direction[0], -direction[1]))

        return continuesDo(newDirections)

    if w <= 1 or w >= bmp.width - 1:
        return callContinus()

    if h <= 1 or h >= bmp.height - 1:
        return callContinus()

    c = bmp.getpixel((w, h))
    similars = GetSimilarPixels(bmp, c, (w, h), [(0,0), fromDirection])
    similarCount = len(similars)
    if similarCount > 1:
        return False

    if 0 == similarCount:
        if callContinus():
            bmp.putpixel((w, h), WHITE_COLOR)
            return True
        return False

    sw, sh = similars[0]
    if 0 == len(directions):
        directions = similars
    elif 1 == len(directions):
        dw, dh = directions[0]
        distW, distH = (sw - dw, sh - dh)
        dist = distW * distW + distH * distH
        if 0 == dist:
            pass
        elif 1 == dist:
            directions.append((sw, sh))
        else:
            return False
    elif (sw, sh) in directions:
        pass
    else:
        return False

    if TraverseLine(bmp, w + sw, h + sh, (-sw, -sh), directions):
        bmp.putpixel((w, h), WHITE_COLOR)
        return True

from functools import partial
def RemoveLine1(bmp, fileName):
    if os.path.isdir(fileName):
        shutil.rmtree(fileName)

    os.mkdir(fileName)

    bmp.save(fileName + ".origin.bmp", "BMP")
    out = Image.new('RGB', (bmp.width, bmp.height))
    # for h in range(HMargin, bmp.height - HMargin):
    #     for w in range(WMargin, bmp.width - WMargin):
    for h in range(1, bmp.height - 1):
        for w in range(1, bmp.width - 1):
            c = bmp.getpixel((w, h))
            if IsBackground(c):
                bmp.putpixel((w, h), WHITE_COLOR)
            else:
                similars = GetSimilarPixels(bmp, c, (w, h), [(0,0)])
                similarCount = len(similars)
                if 0 == similarCount:
                    bmp.putpixel((w, h), WHITE_COLOR)

                if 1 == similarCount:
                    ww, hh = similars[0]
                    if TraverseLine(bmp, w + ww, h + hh, (-ww, -hh), similars):
                        bmp.putpixel((w, h), WHITE_COLOR)
                elif 2 == similarCount:
                    if (1, 0) not in similars or (-1, 1) not in similars:
                        continue
                    ww, hh = similars[0]
                    ww1, hh1 = similars[1]
                    continueDo = partial(TraverseLine, bmp, w + ww1, h + hh1, (-ww1, -hh1))
                    if TraverseLine(bmp, w + ww, h + hh, (-ww, -hh), [similars[0], (-ww1, -hh1)], continueDo):
                        bmp.putpixel((w, h), WHITE_COLOR)
                        #print 'ok=(%d,%d)' % (w, h)
                    else:
                        pass
                        #print 'fail=(%d,%d)' % (w, h)

    bmp.save(fileName + ".out.bmp", "BMP")
if __name__ == '__main__':
    bmpRoot = sys.argv[1]
    for fileName in os.listdir(bmpRoot):
        [stem, ext] = os.path.splitext(fileName)
        if '.bmp' == ext:
            bmpPath = os.path.join(bmpRoot, fileName)
            bmp = Image.open(bmpPath)
            #SimlarAndRemoveLine(bmp, fileName)
            RemoveLine1(bmp, fileName)

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
