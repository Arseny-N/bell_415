#!/bin/bash

step=$1
list_file=$2
sql_file=$3
random=$4

if [ ! $step ]; then step=$((60*60)); fi
if [ ! $list_file ]; then list_file='acid_table.list'; fi
if [ ! $sql_file ]; then sql_file='acid_table.sql';fi
if [ $random ] ; then coma="'";fi

cnt=0
stop=$((24*60*60))
q_start="INSERT INTO rings (ring_duration,profile_id,ring_time)
	 VALUES("
q_duration_end=", '1', '"
q_end="');"
duration='DEFAULT(ring_duration)'

:> $list_file
:> $sql_file


while [ $cnt -le $stop ]; do
    
    if [ $random ]; then
	step=0x`openssl rand 1 -hex`	
	duration=$(date -u -d@$((0x`openssl rand 1 -hex`)) +'%T')		
    fi

    echo  $duration `date -u -d@$cnt +'%T'` >> $list_file

    echo $q_start$coma$duration$coma$q_duration_end$(date -u -d@$cnt +'%T')$q_end >> $sql_file


    cnt=$(( $cnt + $step ))

done


