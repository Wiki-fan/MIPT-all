 #!/bin/bash

BYTE_COUNT=$1

FILENAME="/user/$USER/output.dat"
#echo FILENAME "${FILENAME}"

dd if=/dev/zero of=output.dat  bs=$BYTE_COUNT count=1 2>/dev/null
hdfs dfs -put output.dat $FILENAME

FILEINFO="$(hdfs fsck $FILENAME -files -blocks -locations 2>/dev/null)"
#echo FILEINFO $FILEINFO

BLK_COUNT=` echo "$FILEINFO" | python3 scr.py 1`
#echo BLK_COUNT "$BLK_COUNT"

REALSIZE=0
for ((i=0; i<$BLK_COUNT; i++))
do
	BLKNAME=`echo "$FILEINFO"  | grep -oP "\s*$i\. .*blk_\d*_?" | cut -d : -f 2`
	BLKNAME="${BLKNAME: : -1}"
	#echo BLKNAME $BLKNAME

	NODENAME="$(hdfs fsck -blockId $BLKNAME 2>/dev/null | grep 'Block replica on datanode/rack' | head -n1 | cut -d ' ' -f 5 | cut -d / -f 1)"
	#echo NODENAME $NODENAME

	COMMAND="sudo -u hdfsuser ssh hdfsuser@$NODENAME find / -name $BLKNAME 2>/dev/null"
	#echo $COMMAND
	BLOCKFILE="$( $COMMAND )"
	#echo BLOCKFILE $BLOCKFILE

	BLOCKSIZE="$(sudo -u hdfsuser ssh hdfsuser@$NODENAME du -b $BLOCKFILE | tail -n1 | cut -f1)"
	#echo BLOCKSIZE $BLOCKSIZE

	let REALSIZE=$(($REALSIZE + $BLOCKSIZE))
	#echo REALSIZE $REALSIZE
done

let ANS=$(($REALSIZE - $BYTE_COUNT))
echo $ANS

hdfs dfs -rm /user/$USER/output.dat 2>/dev/null
