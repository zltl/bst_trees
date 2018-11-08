#!/bin/python

import sys

cnt = 0
with open(sys.argv[1]) as lines:
    v = [0, 0, 0, 0]
    for line in lines:
        xline = line.split(",")
        for i in range (0, 4):
            v[i] = int(xline[i]) + v[i]
        cnt = cnt + 1
        if (cnt == 10):
            cnt = 0
            print('{},{},{},{}'.format(v[0]/10, v[1]/10, v[2]/10, v[3]/10))
            v = [0, 0, 0, 0]

