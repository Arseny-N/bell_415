#!/bin/sh

echo '#!/bin/bash'

echo 'bin_dir='$1
echo 'ring_prog='$2
echo '
case "$1" in
	"kill" )
		kill -s SIGTERM $( cat "$pid_file")
		;;
	"state")
		kill -0 $( cat "$pid_file" ) 2&> /dev/null && echo "up" || echo "not runnuing"
		;;
	"rexec")
		kill -s SIGHUP $( cat "$pid_file" )
		;;
	"logs")
	        echo "$logdir"
	        ;;
	"up")
		$bin_dir/run $@
		;;
	"ring")
		$ring_prog $@		
		;;
	"ring1")
		$ring_prog $@		
		;;
	"ring2")
		$ring_prog $@		
		;;
    * )
	echo "Unknown action $1"
	;;
esac
'

