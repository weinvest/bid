import Image
import sys
import color
from collections import OrderedDict
def projectAnalysis(img, area):
    left, top, right, bottom = area
    endIndices = []
    prevProj = 0
    for h in range(top, bottom):
        proj = 0
        for w in range(left, right):
            if not color.isBackground(img.getpixel((w, h))):
                proj = 1
        if proj != prevProj:
            endIndices.append(h)
        prevProj = proj

    if prevProj == 1:
        endIndices.append(bottom)

    results = OrderedDict({})
    for i in range(0, len(endIndices), 2):
        h1 = endIndices[i]
        h2 = endIndices[i + 1]
        veriWeights = []
        wordWeight = 0
        wFrom = left
        for w in range(left, right):
            weight = 0
            for h in range(h1, h2):
                if not color.isBackground(img.getpixel((w, h))):
                    weight +=1

            if weight == 0:
                if wordWeight != 0:
                    veriWeights.append((wordWeight, wFrom, w))
                wordWeight= 0
            else:
                if 0 == wordWeight:
                    wFrom = w
                wordWeight += weight
        results[(h1, h2)] = veriWeights

    return results

def getInfoRects(img, area, colonRatio = 0.25):
    projectResult = projectAnalysis(img, area)
    results = []
    for (h1,h2),veriWeights in projectResult.iteritems():
        wordCount = 0
        sumWeigh = veriWeights[0][0]
        for (wordWeight, wFrom, wTo) in veriWeights[1:]:
            wordCount += 1
            if wordWeight < (sumWeigh/wordCount) * colonRatio:
                rect = (wTo, h1, veriWeights[-1][2], h2)
                results.append(rect)
                break
            else:
                sumWeigh += wordWeight
    return projectResult, results

def getBidRects(img, area):
    projectResult, rects = getInfoRects(img, area)
    lastRect = rects[-1]
    projects = projectResult[(lastRect[1], lastRect[3])]
    for weight, wFrom, wTo in projects[-1::-1]:
        if wFrom >= lastRect[0]:
            print (wFrom, lastRect[1], wTo, lastRect[3])
        else:
            break

def getConfirmRects(img, area, mergeRate = 0.5):
    #0: verify code inputbox
    #1: conform button
    #2: cancel button
    #3: tips area

    projectResult = projectAnalysis(img, area)
    adjustResult = OrderedDict({})
    rects = []
    def appendAdjustResult(weight, h1, h2, wFrom, wTo):
        hFrom = -1
        hTo = h1
        for h in range(h1, h2):
            hasPixel = False
            for w in range(wFrom, wTo):
                if not color.isBackground(img.getpixel((w, h))):
                    hasPixel = True
                    break
            if hasPixel:
                if -1 == hFrom:
                    hFrom = h
                else:
                    hTo = h + 1
        avgWeight = float(weight) / ((wTo - wFrom) * (hTo - hFrom))
        rect = (wFrom, hFrom, wTo, hTo)
        rects.append((avgWeight, rect))
        adjustVeriWeight.append((avgWeight, rect))

    for (h1, h2), veriWeight in projectResult.iteritems():
        adjustVeriWeight = []
        weight, wFrom, wTo = veriWeight[0]
        halfWidth = (h2 - h1) * mergeRate
        for w,f,t in veriWeight[1:]:
            if (f - wTo) < halfWidth:
                wTo = t
                weight += w
            else:
                appendAdjustResult(weight, h1, h2, wFrom, wTo)
                wFrom = f
                wTo = t
                weight = w
        appendAdjustResult(weight, h1, h2, wFrom, wTo)
        adjustResult[(h1, h2)] = adjustVeriWeight

    rects = sorted(rects, key = lambda x : x[0])
    results = [rects[0][1], rects[-2][1], rects[-1][1]]

    rects = sorted(rects, key = lambda x: x[1][2] - x[1][0])
    results.append(rects[-1][1])
    print results

def getResultRects(img, area):
    projectResult = projectAnalysis(img, area)
    for (h1, h2), veriWeight in projectResult.iteritems():
        print("%d,%d=%s" % (h1, h2, veriWeight))

if __name__ == '__main__':
    imgPath = sys.argv[1]
    left, top, right, bottom = (int(i) for i in sys.argv[2:])
    img = Image.open(imgPath)
    #getInfoRects(img, (left, top, right, bottom))
    #getBidRects(img, (left, top, right, bottom))
    #getConfirmRects(img, (left, top, right, bottom))
    getResultRects(img, (left, top, right, bottom))

