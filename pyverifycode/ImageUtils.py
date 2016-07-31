import color

def getSimilarNeighbors(img, c, wh, excludes, sameThreshold = 0.867):
    similars = []
    w, h = wh
    for ww in range(-1, 2):
        for hh in range(-1, 2):
            if (ww, hh) not in excludes and color.isSamePixel(c, img.getpixel((w + ww, h + hh)), sameThreshold):
                similars.append((ww, hh))

    return similars

def getSimilarNeighborsEx(img, c, wh, excludes, sameThreshold = 0.867):
    similars = []
    w, h = wh
    for ww in range(-1, 2):
        for hh in range(-1, 2):
            if (ww, hh) not in excludes:
                dist = color.dist(c, img.getpixel((w + ww, h + hh)))
                if dist > sameThreshold:
                    similars.append((ww, hh, dist))

    return similars
