ADD JAR /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;
add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
SET hive.exec.dynamic.partition.mode=nonstrict;

USE mcs2017013;

SELECT responseCode, SUM(maleCNT), SUM(femaleCNT) FROM 
(SELECT responseCode, IF(gender='male', count(*), 0) as maleCnt, IF(gender='female', count(*), 0) as femaleCnt 
FROM Logs JOIN Users ON (Logs.ip = Users.ip) GROUP BY responseCode, gender) t2 GROUP BY responseCode;

