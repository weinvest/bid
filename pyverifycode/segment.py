import Image
import os
import sys
import color
import ImageUtils

FILEANME = ''
HMargin = 2
WMargin = 2
SAME_ELEMENT_THRESOLD_ALPHA = 0.75
def SelectSameElementEx(origin, me, w, h, c, excludes, thresold = 0.5):
    if w <= WMargin or w >= (origin.width - WMargin):
        return 0

    if h < HMargin or h >= (origin.height - HMargin):
        return 0

    if not hasattr(me, 'left'):
        me.left = w
        me.right = w
        me.top = h
        me.bottom = h
    else:
        if w < me.left:
            me.left = w
        elif w > me.right:
            me.right = w

        if h < me.top:
            me.top = h
        elif h > me.bottom:
            me.bottom = h

    count = 1
    similars = ImageUtils.getSimilarNeighborsEx(origin, c, (w, h), excludes, thresold)
    s1 = []
    distMean = 0
    for (ww, hh, dist) in similars:
        cc = origin.getpixel((w + ww,h ++ hh))
        if color.isBackground(cc):
            continue

        distMean += dist
        cc = origin.getpixel((w + ww, h + hh))
        s1.append((ww, hh, cc))

        origin.putpixel((w + ww, h + hh), color.BLACK_COLOR)
        me.putpixel((w + ww, h + hh), cc)

    if 0 == len(s1):
        return count
    distMean /= len(s1)
    thresold = thresold * SAME_ELEMENT_THRESOLD_ALPHA + (1 - SAME_ELEMENT_THRESOLD_ALPHA) * distMean
    #print('%s thresold=%lf, distMean=%lf' % (FILEANME, thresold, distMean))

    for (ww, hh, cc) in s1:
        count += SelectSameElementEx(origin, me, w + ww, h + hh, cc, [(0, 0), (-ww, -hh)], thresold)

    return count

def SimlarAndRemoveLine(bmp, fileName):
    childBmps = []
    create = True
    for h in range(HMargin, bmp.height - HMargin):
        for w in range(WMargin, bmp.width - WMargin):
            c = bmp.getpixel((w, h))
            if not color.isBackground(c):
                if create:
                    childBmps.append(None)

                childBmps[-1] = Image.new('RGB', (bmp.width, bmp.height))
                me = childBmps[-1]

                me.putpixel((w, h), c)
                bmp.putpixel((w, h), color.BLACK_COLOR)
                count = SelectSameElementEx(bmp, me, w, h, c, [(0,0)])
                create = count > 30
    yLimit = bmp.height / 3
    def posComp(b1 ,b2):
        if (b1.top < yLimit and b2.top > yLimit) or (b1.top > yLimit and b2.top < yLimit):
            return b1.top - b2.top
        return b1.left - b2.left

    childBmps = sorted(childBmps, cmp =  posComp)
    binaryBmps = []
    seq = 1
    import numpy as np
    for child in childBmps:
        binaryBmp = np.zeros((child.bottom - child.top + 1, child.right - child.left + 1), dtype=np.int8)
        for h in range(child.top, child.bottom + 1):
            for w in range(child.left, child.right + 1):
                if not color.isBackground(child.getpixel((w,h))):
                    binaryBmp[h - child.top, w - child.left] = 1
        binaryBmps.append(binaryBmp)
        seq += 1

    return (childBmps, binaryBmps)
