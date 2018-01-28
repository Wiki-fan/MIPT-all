#!/bin/bash

LOC="$( curl --silent -i http://mipt-master.atp-fivt.org:50070/webhdfs/v1$1?op=OPEN 2>/dev/null | python3 scr.py )"
#echo "${LOC}"
COMMAND="${LOC}&length=10"
#echo "${COMMAND}"
curl --silent "${COMMAND}" 2>/dev/null
