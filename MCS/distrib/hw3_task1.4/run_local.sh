#!/usr/bin/env bash

IN="in"
OUT="out.tmp"
NUM_REDUCERS="$1"
CONFIG="--config /home/akhtyamov/conf.empty"

#hdfs dfs -mkdir /tmp/input_pi_directory
echo $2 > "./$IN/input_file"
#for N in `seq 1 $1`; do
#    hdfs dfs -put /tmp/input_file /tmp/input_pi_directory/input_file
#done
# Run MapReduce here
hadoop fs -rm -r -skipTrash ${OUT}

hadoop ${CONFIG} jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
    -D mapreduce.job.name="generate_pi_input" \
    -D mapreduce.job.reduces=$NUM_REDUCERS \
    -files mapper.py,reducer.py \
    -mapper ./mapper.py \
    -reducer ./reducer.py \
    -input $IN \
    -output $OUT



# always remove temporary directories!
#hdfs dfs -rm -r /tmp/input_pi_directory 

rm "./$IN/input_file"
