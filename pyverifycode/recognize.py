import numpy as np
import os
def getEditSimilar(pattern1, pattern2):
    len1 = pattern1.size
    len2 = pattern2.size

    dist = np.zeros((len1 + 1, len2 + 1), dtype = np.int)
    dist[0] = xrange(0, len2 + 1)
    dist[:,0] = xrange(0, len1 + 1)

    for r in xrange(1, len1 + 1):
        for c in xrange(1, len2 + 1):
            minDist = min(dist[r, c - 1], dist[r - 1, c]) + 1
            # if minDist < 0:
            #     print("1111: (%d, %d)min(%d,%d) = %d" % (r, c, dist[r, c-1], dist[r-1, c], minDist))
            if pattern1[0, r - 1] == pattern2[0, c - 1]:
                minDist = min(minDist, dist[r - 1, c - 1])
            else:
                minDist = min(minDist, dist[r - 1, c - 1] + 1)
            dist[r, c] = minDist
    return 1.0 - float(dist[len1, len2]) / max(len1, len2)

class Regonizer(object):

    def __init__(self, patternRoot):
        self.patternDict = {}
        self.recentlyFont = None
        self.loadPatterns(patternRoot)

    def loadPatterns(self, patternRoot):
        if not os.path.exists(patternRoot):
            raise exceptions.Exception("pattern root dir %s doesn't exists" % patternRoot)

        patternFileNames = os.listdir(patternRoot)
        for patternFileName in patternFileNames:
            patternStem, patternExt = os.path.splitext(patternFileName)
            if patternExt == '.vc' and -1 != patternStem.find('-'):
                fontName, fontValue = patternStem.split('-')

                if fontName not in self.patternDict:
                    self.patternDict[fontName] = {}

                fontValues = self.patternDict[fontName]
                fontData = np.loadtxt(os.path.join(patternRoot, patternFileName), np.int8)
                fontData = fontData.reshape((1, fontData.shape[0] * fontData.shape[1]))
                fontValues[fontValue] = fontData

    def computeSelfSimilarMatrix(self):
        def computeOneSimilar(fontData):
            similars = []
            for fontName, fonts in self.patternDict.items():
                for fontValue, fontData1 in fonts.items():
                    similars.append(getEditSimilar(fontData, fontData1))
            return similars

        allSimilars = []
        index = []
        for fontName, fonts in self.patternDict.items():
            for fontValue, fontData in fonts.items():
                index.append(fontName + '-' + fontValue)
                allSimilars.append(computeOneSimilar(fontData))

        import pandas as pd
        similarMatrix = pd.DataFrame(allSimilars, index = index, columns = index)
        return similarMatrix

    def regonizeInFont(self, binaryImg, font):
        maxSimilar = 0.0
        value = None
        for (fontValue, fontData) in font.items():
            thisSimilar = getEditSimilar(fontData, binaryImg)
            if thisSimilar > maxSimilar:
                maxSimilar = thisSimilar
                value = fontValue

        return (value, maxSimilar)

    def regonizeFast(self, binaryImg, thresold = 0.8):
        maxSimilar = 0.0
        value = None
        if self.recentlyFont is not None:
            value, maxSimilar = regonizeInFont(binaryImg, self.recentlyFont)

            if maxSimilar > thresold:
                return (value, maxSimilar)

        matchFont = None
        for fontName, font in self.patternDict.items():
            if self.recentlyFont is not font:
                thisValue, thisSimilar = regonizeInFont(binaryImg, font)
                if thisSimilar > maxSimilar:
                    maxSimilar = thisSimilar
                    value = thisValue
                    matchFont = font

                    if maxSimilar > thresold:
                        self.recentlyFont = matchFont
                        return (value, maxSimilar)

        return (None, 0.0)

    def regonizeSlow(self, binaryImg):
        maxSimilar = 0.0
        value = None
        for fontName, font in self.patternDict.items():
            thisValue, thisSimilar = regonizeInFont(binaryImg, font)
            if thisSimilar > maxSimilar:
                maxSimilar = thisSimilar
                value = thisValue

        return (value, maxSimilar)
