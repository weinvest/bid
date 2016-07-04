import os
import sys
from collections import OrderedDict
def lcsLength(a, b):
    table = [[0] * (len(b) + 1) for _ in xrange(len(a) + 1)]
    for i, ca in enumerate(a, 1):
        for j, cb in enumerate(b, 1):
            table[i][j] = (
                table[i - 1][j - 1] + 1 if ca == cb else
                max(table[i][j - 1], table[i - 1][j]))
    return table[-1][-1]


f = open(sys.argv[1], 'r')
f.readline()

strs = OrderedDict()
for l in f:
    l = l.strip()
    strs[l[-1]] = l.split('+')[0] 

def printAll(strs):
    print('col,' + ','.join(strs.keys()))
    for k,s in strs.items():
        m = []
        l = len(s)
        for k1, j in strs.items():
            m.append(str(float(lcsLength(s,j)) / l))
        
        print('%s,%s' % (k,','.join(m)))

def printLarge(strs, t):
    print('col,' + ','.join(strs.keys()))
    for k,s in strs.items():
        m = []
        l = len(s)
        for k1, j in strs.items():
            v = float(lcsLength(s,j)) / l
            if v > t:
                m.append(str(v))
            else:
                m.append('0')
        
        print('%s,%s' % (k,','.join(m)))

if len(sys.argv) >= 3:
    printLarge(strs, float(sys.argv[2]))
else:
    printAll(strs) 

