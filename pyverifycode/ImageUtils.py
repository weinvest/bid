import color

def getBackgrondNeighbors(img, c, wh):
    blacks = []
    w, h = wh
    for ww in range(w - 1, w + 2):
        for hh in range(h - 1, h + 2):
            if ww >= 0 and ww < img.width and hh >= 0 and hh < img.height and color.isBackground(img.getpixel((ww, hh))):
                blacks.append((ww, hh))
    return blacks

def getSimilarNeighbors(img, c, wh, excludes, sameThreshold = 0.867):
    similars = []
    w, h = wh
    for ww in range(-1, 2):
        for hh in range(-1, 2):
            cc = img.getpixel((w + ww, h + hh))
            if not color.isBackground(cc):
                if (ww, hh) not in excludes and color.isSamePixel(c, cc, sameThreshold):
                    similars.append((ww, hh))

    return similars

def getSimilarNeighborsEx(img, c, wh, excludes, sameThreshold = 0.867):
    similars = []
    w, h = wh
    for ww in range(-1, 2):
        for hh in range(-1, 2):
            if (ww, hh) not in excludes:
                cc = img.getpixel((w + ww, h + hh))
                if not color.isBackground(cc):
                    dist = color.dist(c, cc)
                    if dist > sameThreshold:
                        similars.append((ww, hh, dist))

    return similars
