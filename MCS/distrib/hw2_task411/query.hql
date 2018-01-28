ADD JAR /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;
add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
SET hive.exec.dynamic.partition.mode=nonstrict;
SET hive.exec.max.dynamic.partitions.pernode=300;
SET hive.exec.max.dynamic.partitions=18000;
USE mcs2017013;

DROP TABLE IF EXISTS Logs;
CREATE EXTERNAL TABLE Logs (
    ip STRING,
    time INT,
    request STRING,
    size INT,
    responseCode INT,
    browser STRING
)
ROW FORMAT SERDE 'org.apache.hadoop.hive.serde2.RegexSerDe'
WITH SERDEPROPERTIES (
    "input.regex" = '^(\\S*)\\t\\t\\t(\\d{8})\\S*\\t(\\S*)\\t(\\S*)\\t(\\S*)\\t(\\S*) .*$',
    "timestamp.formats" = 'yyyyMMddHHmmss'
)
STORED AS TEXTFILE
LOCATION '/data/user_logs/user_logs_M';

DROP TABLE IF EXISTS LogsPart;
CREATE EXTERNAL TABLE LogsPart (
    ip STRING,
    request STRING,
    size INT,
    responseCode INT,
    browser STRING
)
PARTITIONED BY (time INT)
STORED AS TEXTFILE;
INSERT OVERWRITE TABLE LogsPart PARTITION (time)
SELECT ip, request, size, responseCode, browser, time FROM Logs;

DROP TABLE IF EXISTS Users;
CREATE EXTERNAL TABLE Users (
    ip STRING,
    browser STRING,
    gender STRING,
    age INT
)
ROW FORMAT DELIMITED FIELDS TERMINATED BY  '\t'
STORED AS TEXTFILE
LOCATION '/data/user_logs/user_data_M';

DROP TABLE IF EXISTS IPRegions;
CREATE EXTERNAL TABLE IPRegions (
    ip STRING,
    region STRING
)
ROW FORMAT DELIMITED FIELDS TERMINATED BY  '\t'
STORED AS TEXTFILE
LOCATION '/data/user_logs/ip_data_M';

