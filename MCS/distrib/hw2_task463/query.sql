ADD JAR /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;
add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
SET hive.exec.dynamic.partition.mode=nonstrict;

USE mcs2017013;

SELECT TRANSFORM(age) USING 'awk "{print 100-$1\;}"' AS to100
FROM Users
LIMIT 10;

