#!/bin/sh

DB_NAME="test_1"
PASSWD="1"
DB_USER="arseni"

cat<<EOF | mysql $DB_NAME -p$PASSWD -u$DB_USER
CREATE TABLE main_table(
       id INT NOT NULL AUTO_INCREMENT, 
       PRIMARY KEY(id),
       name VARCHAR(30), 
       armed BOOL);

CREATE TABLE ring_time_table_1 (
       ind INT NOT NULL AUTO_INCREMENT, 
       PRIMARY KEY(ind), 
       ring_time TIME);


INSERT INTO main_table (name,armed) 
VALUES('m45','1');

INSERT INTO ring_time_table_1 (ring_time) VALUES('8:25');
INSERT INTO ring_time_table_1 (ring_time) VALUES('9:10');
INSERT INTO ring_time_table_1 (ring_time) VALUES('10:5');
INSERT INTO ring_time_table_1 (ring_time) VALUES('8:25');
INSERT INTO ring_time_table_1 (ring_time) VALUES('8:25');
INSERT INTO ring_time_table_1 (ring_time) VALUES('8:25');

EOF

