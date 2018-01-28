import sys
import re

for line in sys.stdin:
    words = re.match("Location: (.*)", line, flags=re.UNICODE)
    if words:
        print(words.group(1).strip(), end="")



