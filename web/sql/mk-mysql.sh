#!/bin/sh

DB_NAME=$1
if [ ! $DB_NAME ]; then DB_NAME='test_2'; fi
PASSWD="1"
DB_USER="arseni"



function run_sql()
{
    
    mysql $DB_NAME -p$PASSWD -u$DB_USER  <  $1
    echo "Run $1 ($?)"
}


run_sql 'tables.sql'

run_sql 'profiles.sql'

run_sql 'rings.sql'

run_sql 'rules.sql'

run_sql 'overrides.sql'



