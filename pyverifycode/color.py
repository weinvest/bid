R = 0
G = 1
B = 2
H = 0
S = 1
V = 2

WHITE_COLOR = (255, 255, 255)
BLACK_COLOR = (0, 0, 0)
def rgb2hsv(c):
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

    return (h, rng * 256.0 / maxC, maxC)

import math
def rgb2hsv1(c):
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

    h *= math.pi / 3.0
    vs = rng / 256.0
    return (vs * math.cos(h), vs * math.sin(h), maxC/256.0)

def dist(c1, c2):
    h1, s1, v1 = rgb2hsv1(c1)
    h2, s2, v2 = rgb2hsv1(c2)
    hDiff = h1 - h2
    sDiff = s1 - s2
    vDiff = v1 - v2
    return math.exp(-math.sqrt(hDiff * hDiff + sDiff * sDiff + vDiff * vDiff))

BACKGROUND_MIN_THRESHOLD = 3
BACKGROUND_MAX_THRESHOLD = 252
BACKGROUND_H_V_THRESHOLD = 70 * 2.56
def isBackground(c):
    if c[R] < BACKGROUND_MIN_THRESHOLD and c[G] < BACKGROUND_MIN_THRESHOLD and c[B] < BACKGROUND_MIN_THRESHOLD:
        return True

    if c[R] > BACKGROUND_MAX_THRESHOLD and c[G] > BACKGROUND_MAX_THRESHOLD and c[B] > BACKGROUND_MAX_THRESHOLD:
        return True

    hsv = rgb2hsv(c)
    return hsv[V] - hsv[S] > BACKGROUND_H_V_THRESHOLD

def isSamePixel(c1, c2, thresold):
    d = dist(c1, c2)
    return d > thresold
