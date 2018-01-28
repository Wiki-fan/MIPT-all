ADD JAR /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;
add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
SET hive.exec.dynamic.partition.mode=nonstrict;

USE mcs2017013;

SELECT browser, COUNT(*) AS cnt from Logs GROUP BY browser ORDER BY cnt DESC;

