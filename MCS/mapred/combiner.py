#!/usr/bin/env python3

import sys

inside = 0
total = 0
for line in sys.stdin:
    arr = line.strip().split(' ')
    x, y = float(arr[0]), float(arr[1])
    print(x, y, file=sys.stderr)
    total += 1
    if x**2+y**2<=1:
        inside += 1

print('Combiner output %.15f'%(inside/total*4), file=sys.stderr)
print("%.15f"%(inside/total*4))
