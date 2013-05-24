#!/bin/sh

echo '#!/bin/bash'

echo 'bin_dir='$1

echo 'ring_prog='$2
echo 'bell_prog='"$1/run"

echo 'log_dir='$3
echo 'pid_file='$4



echo '
pid=$(cat "$pid_file" 2>/dev/null)

case "$1" in
	"kill" )
		$bell_prog --kill # 2>/dev/null
		:
		;;
	"pid" )
		echo $pid
		;;
	"state")
		# kill -0 $pid 2>/dev/null && echo  "up" || echo  "not runnuing" 
		pgrep_pid=$(pgrep run)		
		[ $pgrep_pid ] && [ $pgrep_pid = $pid ] && echo  "up" || echo  "not runnuing" 
		;;
	"rexec")
		$bell_prog --rexec # 2>/dev/null
		:
		;;
	"flush-logs")
		truncate -s0 $log_dir/*
	        ;;
	"creds")		
		id -u; id -g
		;;
	"dbg-log")
		cat $log_dir/dbg
		;;
	"err-log")
		cat $log_dir/err
		;;
	"dbg-log-path")
		echo -n $log_dir/dbg
		;;
	"err-log-path")
		echo -n $log_dir/err
		;;		
	"up")
		shift 1
		$bell_prog $@
		;;
	"ring")
		shift 1
		$ring_prog $@		
		;;
#	"ring1")		
#		$ring_prog $@		
#		;;
#	"ring2")
#		$ring_prog $@		
#		;;
	"help")
		cat $0
		;;
    * )
	echo "Unknown action $1"
	;;
esac'


