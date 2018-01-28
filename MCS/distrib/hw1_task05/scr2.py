#!/bin/python3

import sys
import re

#print(sys.argv, )
#for i, l in enumerate(list(sys.argv)):
#    print(l)
#print("args", sys.argv[0], sys.argv[1])

for line in sys.stdin:
    words = re.match("\s*Total blocks \(validated\):\s*(\d*)", line, flags=re.UNICODE)
    if words:
        print(words.group(1))



