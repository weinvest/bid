import color
import Image
import ImageUtils

def TraverseLine(img, w, h, fromDirection, linePointThresold, directions, count, continuesDo = None):
    def callContinus():
        if continuesDo is None:
            return True

        newDirections = []
        for direction in directions:
            newDirections.append((-direction[0], -direction[1]))

        return continuesDo(newDirections, count)

    if w <= 1 or w >= img.width - 1:
        return callContinus()

    if h <= 1 or h >= img.height - 1:
        return callContinus()

    count += 1
    c = img.getpixel((w, h))
    similars = ImageUtils.getSimilarNeighbors(img, c, (w, h), [(0,0), fromDirection])
    similarCount = len(similars)
    if similarCount > 1:
        if count >= linePointThresold:
            if continuesDo is None:
                img.putpixel((w, h), color.BLACK_COLOR)
                return True
            elif callContinus():
                img.putpixel((w, h), color.BLACK_COLOR)
                return True
        return False

    if 0 == similarCount:
        if callContinus():
            img.putpixel((w, h), color.BLACK_COLOR)
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

    if TraverseLine(img, w + sw, h + sh, (-sw, -sh), linePointThresold, directions, count, continuesDo):
        img.putpixel((w, h), color.BLACK_COLOR)
        return True

def clean(img, linePointThresold = 5):
    for h in range(1, img.height - 1):
        for w in range(1, img.width - 1):
            c = img.getpixel((w, h))
            if color.isBackground(c):
                img.putpixel((w, h), color.BLACK_COLOR)
            else:
                similars = ImageUtils.getSimilarNeighbors(img, c, (w, h), [(0,0)])
                similarCount = len(similars)
                if 0 == similarCount:
                    if len(ImageUtils.getBackgrondNeighbors(img, c, (w, h))) == 7:
                        img.putpixel((w, h), color.BLACK_COLOR)
                    continue

                count = 1
                if 1 == similarCount:
                    ww, hh = similars[0]
                    if TraverseLine(img, w + ww, h + hh, (-ww, -hh), linePointThresold, similars, count):
                        img.putpixel((w, h), color.BLACK_COLOR)
                elif 2 == similarCount:
                    if (1, 0) not in similars or (-1, 1) not in similars:
                        continue
                    ww, hh = similars[0]
                    ww1, hh1 = similars[1]

                    from functools import partial
                    continueDo = partial(TraverseLine, img, w + ww1, h + hh1, (-ww1, -hh1), linePointThresold)
                    if TraverseLine(img, w + ww, h + hh, (-ww, -hh), linePointThresold, [similars[0], (-ww1, -hh1)], count, continueDo):
                        img.putpixel((w, h), color.BLACK_COLOR)
