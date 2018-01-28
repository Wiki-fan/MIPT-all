#!/usr/bin/env python

import sys

sum = 0
n = 0
for line in sys.stdin:
    if line.strip() != '':
        sum += float(line)
        n += 1

#print(sum, n)
print(sum/n)
