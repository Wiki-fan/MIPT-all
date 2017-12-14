#!/usr/bin/env python3

import re, sys, datetime

#print("============ Debug: mapper started", file=sys.stderr)

reg = re.compile('-?\d*\t(\d*)\t(\S*)\t.*')	
for line in sys.stdin:
    match = reg.match(line)
    if not match:
        continue
    dt = datetime.datetime.fromtimestamp(float(match.group(1)))
    site = match.group(2)[7:]
    if site.startswith('www.'):
        site = site[4:]
    site = site.split('/')[0]
    print('%s \t %s'%(site, 0 if dt.weekday()<5 else 1))

#print("I'm done", file=sys.stderr)

