ADD JAR /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;
add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
SET hive.exec.dynamic.partition.mode=nonstrict;

USE mcs2017013;

SELECT TRANSFORM(ip, time, request, size, responseCode, browser) USING 'sed -e "s/Safari/Chrome/"' AS ip, time, request, size, responseCode, browser
FROM Logs
LIMIT 10;

