#!/bin/sh

DB_NAME=$1
if [ ! $DB_NAME ]; then DB_NAME='test_2'; fi
PASSWD="1"
DB_USER="arseni"



function run_sql()
{
    
    for script in $@; do
    	mysql $DB_NAME -p$PASSWD -u$DB_USER  <  $script
    	echo "Run $script ($?)"
    done
}

shift

run_sql 'tables.sql' 'profiles.sql' 'rings.sql' 'rules.sql' 'overrides.sql' $@




