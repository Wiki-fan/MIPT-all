import sys
import re

for line in sys.stdin:
    words = re.match("Location: http://(.*):\d*/", line, flags=re.UNICODE)
    if words:
        print(words.group(1))



