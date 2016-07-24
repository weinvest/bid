import Image
import os
import sys

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
        return False

    if c[R] > BACKGROUND_MAX_THRESHOLD and c[G] > BACKGROUND_MAX_THRESHOLD and c[B] > BACKGROUND_MAX_THRESHOLD:
        return False

    return True

SAME_PIXEL_H_THRESOLD = 25
SAME_PIXEL_SV_THRESOLD = 35000
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
def SelectSameElementEx(origin, me, w, h, count):
    if w <= WMargin or w >= (origin.width - WMargin):
        return

    if h < HMargin or h >= (origin.GetHeight() - HMargin):
        return

    tl = False, t = False, tr = False
    l = False, r = False
    bl = False, b = False, br = False

    c = me.getpixel(w, h)
    def spreadPixel(ww, hh):
        cc = origin.getpixel(ww, hh)
        if IsSamePixel(c, cc):
            origin.SetPixel(ww, hh, WHITE_COLOR)
            me.SetPixel(ww, hh, cc)
            count += 1
            return True
        return False

    tl = spreadPixel(w - 1, h - 1)
    t = spreadPixel(w, h - 1)
    tr = spreadPixel(w + 1, h - 1)
    l = spreadPixel(w - 1, h)
    r = spreadPixel(w + 1, h)
    bl = spreadPixel(w - 1, h + 1)
    b = spreadPixel(w, h + 1)
    br = spreadPixel(w + 1, h + 1)

    if tl:SelectSameElementEx(origin, me, w - 1, h - 1, count)
    if t: SelectSameElementEx(origin, me, w, h - 1, count)
    if tr: SelectSameElementEx(origin, me, w + 1, h - 1, count)
    if l: SelectSameElementEx(origin, me, w - 1, h, count)
    if r: SelectSameElementEx(origin, me, w + 1, h, count)
    if bl: SelectSameElementEx(origin, me, w - 1, h + 1, count)
    if b:SelectSameElementEx(origin, me, w, h + 1, count)
    if br: SelectSameElementEx(origin, me, w + 1, h + 1, count)

def SimlarAndRemoveLine(bmp, fileName):
    if not os.path.isdir(fileName):
        os.mkdir(fileName)

    bmp.save(fileName + "/origin.bmp", "BMP")
    childBmps = []
    for h in range(HMargin, bmp.heigh - HMargin):
        for w in range(WMargin, bmp.GetWidth() - WMargin):
            c = bmp.getpixel((w, h))
            if not IsBackground(c):
                if create:
                    childBmps.append(None)

                childBmps[-1] = Image.new('rgb', (bmp.width, bmp.heigh))
                me = childBmps[-1]

                me.setpixel((w, h), c)
                bmp.SetPixel((w, h), WHITE_COLOR)
                count = 1
                SelectSameElementEx(bmp, me, w, h, count)
                create = count > 30

    bmp.save(fileName + "/out.bmp", "BMP")
    seq = 1
    for child in childBmps:
        child.save(fileName + "/" + str(seq) + ".bmp", "BMP")
        seq += 1

bmpPath = sys.argv[1]
bmp = Image.open(bmpPath)
rng = [int(i) for i in sys.argv[2].split(',')]
hsvs = ToHSVList(bmp, rng[0], rng[1], rng[2], rng[3])

import pandas as pd
dHSV = pd.DataFrame(hsvs, columns = list('hsv'))
dHSV = dHSV[dHSV.h != 0]
dHSV['h'].hist(bins = 72)

import matplotlib.pyplot as pl
pl.show()