#!/bin/sh

BIN_DIR=$1
LOG_DIR=$2
PID_FILE=$3

echo '#!/bin/bash'

echo 'logdir='$LOG_DIR
echo 'pid_file='$PID_FILE
echo 'bin_dir='$BIN_DIR

echo '
case "$1" in
    "kill" )
	kill -s SIGTERM $( cat "$pid_file")
	;;
    "is-up")
	kill -0 $( cat "$pid_file" ) 2&> /dev/null && echo "runnung" || echo "not runnuing"
	;;
    "rexec")
	kill -s SIGHUP $( cat "$pid_file" )
	;;
    "logs")
        echo "$logdir"
        ;;
    "up")
	$bin_dir/run --dump_config  --log_err $logdir/run-err.log --log_out $logdir/run-dbg.log --ring_prog $bin_dir/ringer --pid_file $pid_file
	;;
    * )
	echo "Unknown action $1"
	;;
esac
'

