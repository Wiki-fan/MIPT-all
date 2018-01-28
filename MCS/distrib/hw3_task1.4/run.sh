#!/usr/bin/env bash

IN="/tmp/input_pi_directory"
OUT="out.tmp"
NUM_REDUCERS="$1"
CONFIG=""

hdfs dfs -mkdir /tmp/input_pi_directory
echo $2 > /tmp/input_file
for N in `seq 1 $1`; do
    hdfs dfs -put /tmp/input_file /tmp/input_pi_directory/input_file${N}
done
rm /tmp/input_file
# Run MapReduce here
hadoop fs -rm -r -skipTrash ${OUT}

hadoop ${CONFIG} jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
    -D mapreduce.job.name="generate_pi_input" \
    -D mapreduce.job.reduces=1 \
    -files mapper.py,reducer.py,combiner.py \
    -mapper ./mapper.py \
    -combiner ./combiner.py \
    -reducer ./reducer.py \
    -input $IN \
    -output $OUT



# always remove temporary directories!
hdfs dfs -rm -r /tmp/input_pi_directory 

hadoop fs -cat ${OUT}/part-00000 | head

