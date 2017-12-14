#!/bin/bash

FILENAME="$(hdfs fsck -blockId $1 2>/dev/null | grep '^Block belongs to:' | cut -d : -f 2)"
NODENAME="$(hdfs fsck -blockId $1 2>/dev/null | grep 'Block replica on datanode/rack' | head -n1 | cut -d ' ' -f 5 | cut -d / -f 1)"
#echo $NODENAME
#echo "${FILENAME}"
NODEIP="$(hdfs fsck $FILENAME -files -blocks -locations 2>/dev/null | grep $1| grep -oP 'DatanodeInfoWithStorage\[(.*?),' | grep -oP '([\d*\.*]+)' | head -n 1 | cut -d ' ' -f1)"
#echo $NODEIP
#echo "find / -name $1 2>/dev/null"
BLOCKFILE="$( sudo -u hdfsuser ssh hdfsuser@$NODEIP 'find / -name $1 2>/dev/null' )"
#echo $BLOCKFILE
#echo $NODENAME:
#echo "sudo -u hdfsuser ssh hdfsuser@$NODEIP 'find / -name $1 2>/dev/null'"
echo $NODENAME:`sudo -u hdfsuser ssh hdfsuser@$NODEIP "find / -name $1 2>/dev/null"`
#echo "$(sudo -u hdfsuser ssh hdfsuser@$NODEIP 'find / -name $1 2>/dev/null' 2>&1)"
#echo $BLOCKFILE
