#!/usr/bin/env python

import sys

sum0 = 0
sum1 = 0
current_key = None

for line in sys.stdin:
    arr = line.split('\t')
    key, val = arr
    if current_key != key:
        if current_key:
            print("%s\t%d\t%d" % (current_key, sum0, sum1))
        sum0 = 0
        sum1 = 0
        current_key = key
    if int(val) == 0:
        sum0 += 1
    else:
        sum1 += 1

if current_key:
    print("%s\t%d\t%d" % (current_key, sum0, sum1))

