#!/bin/bash

STEP=$1
DURATION=$2
PROFILE_ID=$3

if [[ ! $STEP || ! $DURATION || ! $PROFILE_ID ]]; then echo "Usage $0 {STEP|random} {DURATION|default|random} PROFILE_ID"; exit 1;fi

cnt=0

duration=\'$DURATION\'
step=$STEP

[ $DURATION = "default" ] && duration="DEFAULT(ring_duration)"

echo -e 'INSERT INTO rings (ring_duration,profile_id,ring_time)\nVALUES'

while true; do

	[ $DURATION = "random" ] && duration=\'$(date -u -d@$((0x`openssl rand 1 -hex`)) +'%T')\'
	[ $STEP = "random" ] && step=0x`openssl rand 1 -hex`
	
	echo -en "\t($duration,'$PROFILE_ID','$(date -u -d@$cnt +'%T')')"
	
    	cnt=$(( $cnt + $step ))
    	
    	if [ $cnt -gt 86400 ] ; then 
    		echo ";"
    		break;
    	fi
    	echo ","
done


