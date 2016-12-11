import math
import numpy as np
import pandas as pd
#params:
#    dist: a pandas.DataFrame object with v1,v2 as it's index and dist as it's value
#return:
#    vertexProperties: a pandas.DataFrame object with index=union(v1,v2)
def createVertexProperties(dist):
    vertexIds = np.union1d(dist.index.get_level_value(0), dist.index.get_level_value(1))
    numOfVertics = len(vertexIds)
    maxDelta = dist.dist.max()
    return pd.DataFrame(index=vertexIds
                        , data={'rtho': np.zeros((numOfVertics, 1)),
                                'delta': [maxDelta] * numOfVertics,
                                'gamma': [0] * numOfVertics,
                                'cluster': [-1] * numOfVertics})

def computeDistThreshold(vertexProperties, dist, avgPercentOfNeighbors):
    numOfVertices = len(vertexProperties)
    position = int(numOfVertices * numOfVertices * 0.5 * avgPercentOfNeighbors)
    threshold = sorted(dist.dist)[position]

    return threshold

def computeRtho(vertexProperties, dist, distThreshold):
    deltaRtho = np.exp(-dist.dist/distThreshold)
    vertexProperties.rtho += deltaRtho.reset_index(level='v2', drop=True)
    vertexProperties.rtho += deltaRtho.reset_index(level='v1', drop=True)

def computeDelta(vertexProperties, dist):
    numOfVertices = len(vertexProperties)
    vertexProperties.sort_values('rtho')
    vertexProperties.map()

def computeGamma(vertexProperties, plot = False):
    vertexProperties.gamma = vertexProperties.rtho * vertexProperties.delta

    if plot:
        vertexProperties.plot(x = 'rho', y = 'delta')
        vertexProperties.plot(x = '')

def

