import sys
import re

for line in sys.stdin:
    words = re.match("\s*Total blocks \(validated\):\s*(\d*)", line, flags=re.UNICODE)
    if words:
        print(words.group(1))



