#!/usr/bin/env bash

IN="/data/user_events_part"
OUT="out"
CONFIG=""

# Run MapReduce here
hadoop fs -rm -r -skipTrash ${OUT}
hadoop fs -rm -r -skipTrash ${OUT}.tmp

hadoop ${CONFIG} jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
    -D mapreduce.job.name="count visits step 1" \
    -D mapreduce.job.reduces=1 \
    -files mapper.py,reducer.py \
    -mapper ./mapper.py \
    -reducer ./reducer.py \
    -input $IN \
    -output $OUT.tmp

hadoop jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
    -D mapreduce.job.name="count visits step 2" \
    -D mapreduce.job.reduces=1 \
    -D mapreduce.partition.keycomparator.options=-k2,2nr \
    -D mapreduce.job.output.key.comparator.class=org.apache.hadoop.mapreduce.lib.partition.KeyFieldBasedComparator \
    -files id.py \
    -mapper id.py \
    -reducer id.py \
    -input ${OUT}.tmp \
    -output ${OUT}

hadoop fs -cat ${OUT}/part-00000
