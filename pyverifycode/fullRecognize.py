import color
import os
import numpy as np
class FontPattern(object):
    def __init__(self, pattern):
        elements = []
        top, left = -1, -1
        for h in xrange(0, pattern.shape[0]):
            for w in xrange(0, pattern.shape[1]):
                if 0 != pattern[h, w]:
                    if -1 == top:
                        top, left = h, w
                        elements.append((0, 0))
                    else:
                        elements.append((w - left, h - top))

        self.elements = elements
        self.height, self.width = pattern.shape

class FullRecognizer(object):
    def __init__(self, patternRoot):
        self.patternDict = {}
        self.loadPatterns(patternRoot)

    def loadPatterns(self, patternRoot):
        if not os.path.exists(patternRoot):
            raise exceptions.Exception("pattern root dir %s doesn't exists" % patternRoot)

        minWidth, minHeight = (10000, 10000)
        patternFileNames = os.listdir(patternRoot)
        for patternFileName in patternFileNames:
            patternStem, patternExt = os.path.splitext(patternFileName)
            if patternExt == '.vc' and -1 != patternStem.find('-'):
                fontName, fontValue = patternStem.split('-')

                if fontName not in self.patternDict:
                    self.patternDict[fontName] = {}

                fontValues = self.patternDict[fontName]
                fontData = np.loadtxt(os.path.join(patternRoot, patternFileName), np.int8)
                fontData = FontPattern(fontData)
                fontValues[fontValue] = fontData

                minWidth = min(minWidth, fontData.width)
                minHeight = min(minHeight, fontData.height)

        self.minWidth, self.minHeight = minWidth, minHeight
        print self.minWidth, self.minHeight
    def compareScanResult(self, ret1, ret2):
        value1, pattern1, loseCount1, maxCount1, w1, h1 = ret1
        value2, pattern2, loseCount2, maxCount2, w2, h2 = ret2
        if value1 is None:
            return ret2

        if value2 is None:
            return ret1

        if (loseCount1 < loseCount2 or (loseCount1 == loseCount2 and maxCount1 > maxCount2)):
            return ret1

        return ret2

    def doScanPatterns(self, img, wh):
        w, h = wh
        minLoseCount = 10000
        maxCount = 0
        ret = (None, None, None, None, None, None)
        candidates = []
        candidateColors = {}
        # c = img.getpixel(wh)
        for fontName, fontValues in self.patternDict.items():
            for value, fontPattern in fontValues.items():
                count = 0
                loseCount = 0
                tolerateCount = int(len(fontPattern.elements) * 0.1)
                colors = []
                for ww, hh in fontPattern.elements:
                    count += 1
                    rw, rh = w + ww, h + hh
                    if rw >= img.width or rw < 0 or rh < 0:
                        loseCount += 1
                    elif rh >= img.height:
                        loseCount += len(fontPattern.elements) - count
                        break
                    elif color.isBackground(img.getpixel((rw, rh))):
                        loseCount += 1

                    if loseCount > tolerateCount:
                        break
                    else:
                        colors.append(color.rgb2hsv(img.getpixel((rw, rh))))

                if loseCount <= tolerateCount:
                    ret1 = (value, fontPattern, loseCount, maxCount, w, h)
                    ret = self.compareScanResult(ret, ret1)
                    # if (loseCount < minLoseCount or (loseCount == minLoseCount and count > maxCount)):
                    #     maxCount = count
                    #     minLoseCount = loseCount
                    #     ret = (value, fontPattern, loseCount, maxCount)
                    candidates.append((loseCount, value, fontPattern))
                    # candidateColors[value] = std

        # if len(candidates) >= 2:
        #     print('(%d,%d)=%s:---:%s' % (w, h, str(candidates), str(candidateColors)))
        #
        # if ret[0] is not None:
        #     print('r:(%d,%d)=(%d, %s):---:%s' % (w, h, minLoseCount, ret[0], str(candidateColors[ret[0]])) )
        return ret
    def scanPatterns(self, img, wh):
        ret = self.doScanPatterns(img, wh)
        if ret[0] is not None:
            w, h = wh
            for wh1 in [(w+1,h), (w, h+1), (w-1,h+1), (w+1,h+1), (w + 2, h), (w + 2, h + 1), (w + 2, h + 2), (w + 1, h + 2), (w, h + 2), (w - 1, h + 2)]:
                ret = self.compareScanResult(ret, self.doScanPatterns(img, wh1))
            print ret

        return ret
    def cleanPattern(self, img, pattern, wh):
        w, h = wh
        for ww, hh in pattern.elements:
            if hh < pattern.height * 0.75:
                img.putpixel((w + ww, h + hh), color.BLACK_COLOR)

    def __valueCompare(self, v1, v2):
        hLimit = 16
        w1, h1 = v1[1]
        w2, h2 = v2[1]
        if (h1 < hLimit and h2 < hLimit) or (h1 > hLimit and h2 > hLimit):
            return w1 - w2

        return h1 - h2

    def regonize(self, img, topLeft, bottomRight):
        left, top = topLeft
        right, bottom = bottomRight

        width = right - left - self.minWidth
        height = bottom - top - self.minHeight / 2
        values = []
        maxHeight = top + height + 1
        maxWidth = left + width + 1
        #print('(%d,%d)=>(%d,%d)' % (width, height, maxWidth, maxHeight))
        for h in xrange(top, maxHeight):
            for w in xrange(left, maxWidth):
                if not color.isBackground(img.getpixel((w, h))):
                    value, pattern, loseCount, count, ww, hh = self.scanPatterns(img, (w, h))
                    if value is not None:
                        values.append((value, (ww, hh)))
                        self.cleanPattern(img, pattern, (ww, hh))

        values = sorted(values, cmp =  self.__valueCompare)
        values = [i[0] for i in values]
        return values
