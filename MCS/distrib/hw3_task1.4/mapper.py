#!/usr/bin/env python3

import random, sys

#print("============ Debug: mapper started", file=sys.stderr)

N = None
N = int(sys.stdin.readline())
	
for i in range(N):
    print("%.10f %.10f"%(random.uniform(-1, 1), random.uniform(-1, 1)))
#    print("%.15f %.15f"%(random.uniform(-1, 1), random.uniform(-1, 1)), file=sys.stderr)

#print("I'm done", file=sys.stderr)

