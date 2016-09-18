import color
import os
import numpy as np
import exceptions
class FontPattern(object):
    def __init__(self, name, pattern):
        self.name = name
        self.height, self.width = pattern.shape
        elements = []
        top, left = self.height / 2, self.width / 2
        for h in xrange(0, pattern.shape[0]):
            for w in xrange(0, pattern.shape[1]):
                if 0 != pattern[h, w]:
                    ww, hh = w - left, h - top
                    elements.append((ww, hh))

        self.elements = elements

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
                fontData = FontPattern(patternStem, fontData)
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

        if maxCount1 == maxCount2:
            return ret1 if loseCount1 < loseCount2 else ret2

        if maxCount2 > maxCount1:
            ret1, ret2 = ret2, ret1
            value1, pattern1, loseCount1, maxCount1, w1, h1 = ret1
            value2, pattern2, loseCount2, maxCount2, w2, h2 = ret2

        diff = float(loseCount1) / maxCount1 - float(loseCount2) / maxCount2
        if diff < -1e-7:
            return ret1

        loseDiff = float(loseCount1 - loseCount2) / float(maxCount1 - maxCount2)
        if loseDiff < 0.12:
            return ret1

        return ret2

    def doScanPatterns(self, img, wh):
        w, h = wh
        ret = (None, None, None, None, None, None)
        candidates = []
        for fontName, fontValues in self.patternDict.items():
            for value, fontPattern in fontValues.items():
                count = 0
                loseCount = 0
                tolerateCount = int(len(fontPattern.elements) * 0.2)
                #colors = []
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
                    # elif rw >= 0 and rw < img.width and rh >=0 and rh < img.height:
                    #     colors.append(color.rgb2hsv(img.getpixel((rw, rh))))
                # if fontName == 'seal' and value == '5':
                #     print loseCount,len(fontPattern.elements)

                if loseCount <= tolerateCount:
                    ret1 = (value, fontPattern, loseCount, len(fontPattern.elements), w, h)
                    ret = self.compareScanResult(ret, ret1)
                    candidates.append((loseCount, value, fontPattern))

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

    def regonizeEx(self, img):
        import numpy as np
        columns = [25, 30, 41, 45, 57, 62, 73, 77]
        rows = [10, 17, 30, 36]

        values = []
        for idxH in range(0, len(rows) / 2):
            for idxW in range(0, len(columns) / 2):
                left = columns[2 * idxW]
                right = columns[2 * idxW + 1]
                top = rows[2 * idxH]
                bottom = rows[2 * idxH + 1]

                ret = (None, None, None, None, None, None)
                for boxH in range(top, bottom + 1):
                    for boxW in range(left, right + 1):
                        retCur = self.doScanPatterns(img, (boxW, boxH))
                        ret = self.compareScanResult(ret, retCur)
                value, pattern, loseCount, count, ww, hh = ret
                if value is not None:
                    print('(%d,%d,%d,%d)=%s, %s @(%d,%d)' % (left, right, top, bottom, value, pattern.name, ww, hh))
                else:
                    print('(%d,%d,%d,%d)=%s, %s @(%d,%d)' % (left, right, top, bottom, None, None, -1, -1))
                if value is not None:
                    values.append((value, (ww, hh)))

        values = [i[0] for i in values]
        return values
