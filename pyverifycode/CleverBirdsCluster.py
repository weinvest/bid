import math
import numpy as np
def createVertexProperties(dist):
    pass

def computeDistThreshold(vertexProperties, dist, avgPercentOfNeighbors):
    numOfVertices = len(vertexProperties)
    position = int(numOfVertices * numOfVertices * 0.5 * avgPercentOfNeighbors)
    threshold = sorted(dist[:,3])[position]

    return threshold


def computeRtho(vertexProperties, dist, distThreshold):
    for v1Id, v2Id, dist in dist:
        v1 = vertexProperties[v1Id]
        v2 = vertexProperties[v2Id]

        deltaRtho = math.exp(-dist/distThreshold)
        v1.rtho += deltaRtho
        v2.rtho += deltaRtho

def computeDelta(vertexProperties, dist):
    def vertexComparae(v1, v2):
        return v1.rtho - v2.rtho

    sortedVertices = sorted(vertexProperties, vertexComparae)
    for v in sortedVertices[1:]:
        pass

