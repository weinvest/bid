#!/usr/bin/env python

import os
import sys
import re
f = sys.stdin
if 2 == len(sys.argv):
    f = open(sys.argv[1], 'r')
lines = f.readlines()
last4 = lines[-1]
jpgFileReg = re.compile("\w+\.jpg")
failedList = jpgFileReg.findall(last4)

cResult = []
for i in range(0, 16):
    cResult.append(set())

for i in xrange(2, len(lines) -6, 9):
    fileName = lines[i].strip()
    if fileName not in failedList:
        for j in range(0, 8):
            l = lines[i + j + 1]
            centStart = l.rfind('(') + 1
            centEnd = l.rfind(')')
            center = l[centStart: centEnd].split(',')
            cw = int(center[0])
            ch = int(center[1])
            cResult[2 * j].add(cw)
            cResult[2 * j + 1].add(ch)
            if 0 == j and cw == 40:
                for k in range(0, 9):
                    print(lines[i + k])


for i in range(0, 8):
    print('%d:' % i)
    w = cResult[2 * i]
    h = cResult[2 * i + 1]
    print w
    print h
    print min(w), max(w), min(h), max(h)
    


