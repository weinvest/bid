import math
import numpy as np
import pandas as pd
#params:
#    dist: a pandas.DataFrame object with v1,v2 as it's index and dist as it's value
#return:
#    vertexProperties: a pandas.DataFrame object with index=union(v1,v2)
def createVertexProperties(dist):
    vertexIds = np.union1d(dist.index.get_level_values(0), dist.index.get_level_values(1))
    numOfVertics = len(vertexIds)
    maxDelta = dist.dist.max()
    return pd.DataFrame(index=vertexIds
                        , data={'rtho': [0] * numOfVertics,
                                'delta': [maxDelta] * numOfVertics,
                                'nearestVertexId': [-1] * numOfVertics,
                                'gamma': [0] * numOfVertics,
                                'cluster': [-1] * numOfVertics,
                                'halo': [0] * numOfVertics})

def computeDistThreshold(vertexProperties, dist, avgPercentOfNeighbors = 0.02):
    numOfVertices = len(vertexProperties)
    position = int(numOfVertices * numOfVertices * 0.5 * avgPercentOfNeighbors)
    sortedDist = sorted(dist.dist)
    threshold = sortedDist[int(len(sortedDist) * 0.1)]

    return threshold

def computeRtho(vertexProperties, dist, distThreshold):
    deltaRtho = np.exp(-dist.dist/distThreshold)
    vertexProperties.rtho += deltaRtho.groupby(level='v1').sum()
    vertexProperties.rtho += deltaRtho.groupby(level='v2').sum()

def computeDelta(vertexProperties, dist):
    numOfVertices = len(vertexProperties)
    vertexProperties.sort_values('rtho')

    itRows = vertexProperties.iterrows()
    nbVertexIds = [itRows.next()[0]]
    for vertexId, pros in itRows:
        print dist.loc[vertexId,:]

        nearest = dist.loc[dist.index.get_level_values(0).isin(nbVertexIds) \
                           | dist.index.get_level_values(1).isin(nbVertexIds)].dist.idxmin()
        pros.nearestVertexId = nearest[0]
        pros.delta = nearest[1].dist
        nbVertexIds.append(vertexId)

def computeGamma(vertexProperties, plot = False):
    vertexProperties.gamma = vertexProperties.rtho * vertexProperties.delta

    if plot:
        vertexProperties.plot(x = 'rho', y = 'delta')
        vertexProperties.plot(x = '', y = 'gamma')

def assignCluster(vertexProperties, clusters):
    for vertexId, pros in vertexProperties.iterrows():
        if pros.cluster == '-1':
            if vertexId[0] in clusters:
                pros.cluster = clusters.index(vertexId[0])
            else:
                pros.cluster = vertexProperties.loc[pros.nearestVertexId].cluster
            pros.halo = vertexId[0]

def computeHalo(vertexProperties, clusters, dist, distThreshold):
    clusterBorderAvgRho = {c:0 for c in clusters}
    for edge, d in dist.iterrows():
        v1Id, v2Id = edge
        if d < distThreshold:
            v1 = vertexProperties.loc[v1Id]
            v2 = vertexProperties.loc[v2Id]
            avgRho = (v1.rtho + v2.rtho)
            if avgRho > clusterBorderAvgRho[v1.cluster]:
                clusterBorderAvgRho[v1.cluster] = avgRho

            if avgRho > clusterBorderAvgRho[v2.cluster]:
                clusterBorderAvgRho[v2.cluster] = avgRho

    def assignHalo(r):
        if r.rtho < clusterBorderAvgRho[r.cluster]:
            r.halo = 0

    vertexProperties.apply(assignHalo, axis = 1)

def statCluster(vertexProperties):
    return vertexProperties.groupby(vertexProperties.cluster).count().halo

import sys
import os
import Image
import numpy as np
import pandas as pd
import color

def testImage():
    def getVertexId(bmp, p):
        return bmp.width * p[1] + p[0]

    def id2position(bmp, id):
        return id % bmp.width, id / bmp.width

    rect = [1, 49, 20, 85]
    bmpPath = sys.argv[1]
    bmp = Image.open(bmpPath)

    fromVertex = []
    toVertex = []
    dicDist = []
    for h in range(rect[0], rect[1]):
        for w in range(rect[2], rect[3]):
            vId = getVertexId(bmp, (w,h))
            c = bmp.getpixel((w,h))
            neighbors =[(w+1, h), (w-1,h+1), (w,h+1), (w+1,h+1)]

            fromVertex.extend([vId] * 4)
            toVertex.extend([getVertexId(bmp, v) for v in neighbors])
            dicDist.extend([color.dist(c, bmp.getpixel(v)) for v in neighbors])

    dist = pd.DataFrame({'v1': fromVertex, 'v2': toVertex, 'dist': dicDist})
    dist.set_index(['v1', 'v2'], inplace=True)
    #print dist

    vertexProperties = createVertexProperties(dist)
    distThreshold = computeDistThreshold(vertexProperties, dist, 0.00002)
    computeRtho(vertexProperties, dist, distThreshold)
    computeDelta(vertexProperties, dist)
    computeGamma(vertexProperties, dist)

from scipy import spatial
if __name__ == '__main__':
    fileName = sys.argv[1]
    location = []
    label = []
    for line in open(fileName, "r"):
        items = line.strip("\n").split(",")
        label.append(int(items.pop()))
        tmp = []
        for item in items:
            tmp.append(float(item))
        location.append(tmp)
    location = np.array(location)
    label = np.array(label)
    length = len(location)

    print spatial.distance.pdist(location)
    #Caculate distance
    v1 = []
    v2 = []
    dist = []
    begin = 0
    while begin < length-1:
        end = begin + 1
        while end < length:
            dd = np.linalg.norm(location[begin]-location[end])
            v1.append(begin)
            v2.append(end)
            dist.append(dd)

    dist = pd.DataFrame({'v1': v1, 'v2': v2, 'dist': dist})
    dist.set_index(['v1', 'v2'], inplace=True)
    vertexProperties = createVertexProperties(dist)
    distThreshold = computeDistThreshold(vertexProperties, dist)
    computeRtho(vertexProperties, dist, distThreshold)
    computeDelta(vertexProperties, dist)
    computeGamma(vertexProperties, dist)