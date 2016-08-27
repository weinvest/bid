import Image
import os
import sys
import color
import ImageUtils

FILEANME = ''
HMargin = 2
WMargin = 2
SAME_ELEMENT_THRESOLD_ALPHA = 1
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

def SimlarAndRemoveLine(bmp, fileName, thresold = 0.5):
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
                count = SelectSameElementEx(bmp, me, w, h, c, [(0,0)], thresold)
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

def Segment(bmp):
    import numpy as np
    bmpArray = np.array(bmp)
    print bmpArray.sum(axis = 0).sum(axis = 1)
    temp = map(lambda x: 0 if x < 300 else 1, bmpArray.sum(axis = 0).sum(axis = 1))
    splitIndices = []
    for i in range(1, len(temp)):
        if (temp[i] - temp[i - 1]) != 0:
            splitIndices.append(i)
    results = []
    for i in range(0, len(splitIndices), 2):
        start, end = splitIndices[i], splitIndices[i + 1]
        left = end
        right = -1
        top = bmp.height
        bottom = 0
        binaryImg = np.zeros((bmp.height, bmp.width))
        for h in range(0, bmp.height):
            for w in range(start, end):
                if not color.isBackground(bmp.getpixel((w, h))):
                    binaryImg[h, w] = 1
                    if w < left:
                        left = w
                    if w > right:
                        right = w
                    if h < top:
                        top = h
                    if h > bottom:
                        bottom = h
        print top, bottom, left, right
        result = binaryImg[top:bottom + 1, left:right + 1]
        results.append(result)
    return results

def HorizontalGradient(bmp):
    gradient = Image.new('RGB', (bmp.width, bmp.height))
    for h in range(0, bmp.height):
        for w in range(1, bmp.width - 1):
            if color.isBackground(bmp.getpixel((w - 1, h))):
                leftC = (0, 0, 0)
            else:
                leftC = bmp.getpixel((w - 1, h))

                if color.isBackground(bmp.getpixel((w + 1, h))):
                    rightC = (0, 0, 0)
                else:
                    rightC = bmp.getpixel((w + 1, h))

                gradient.putpixel((w, h), tuple(map(lambda l,r: (l + r)/2, leftC, rightC)))
    import numpy as np
    xx = np.array(gradient)
    d = xx.sum(axis = 0)
    for i in range(0, len(d)):
        x,y,z = d[i]
        print('%2d: %d' % (i, x*x+y*y+z*z))
    return gradient

def VerticalGradient(bmp):
    gradient = Image.new('RGB', (bmp.width, bmp.height))
    for h in range(1, bmp.height - 1):
        for w in range(0, bmp.width):
            if color.isBackground(bmp.getpixel((w, h - 1))):
                topC = (0, 0, 0)
            else:
                topC = bmp.getpixel((w, h - 1))

            if color.isBackground(bmp.getpixel((w, h + 1))):
                bottomC = (0, 0, 0)
            else:
                bottomC = bmp.getpixel((w, h + 1))

            gradient.putpixel((w, h), tuple(map(lambda l,r: (l + r)/2, topC, bottomC)))
    import numpy as np
    xx = np.array(gradient)
    xx.sum(axis = 1)
    return gradient
