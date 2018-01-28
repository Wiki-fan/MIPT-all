#!/bin/bash

curl --silent -i "http://mipt-master.atp-fivt.org:50070/webhdfs/v1$1?op=OPEN" | python scr.py 

