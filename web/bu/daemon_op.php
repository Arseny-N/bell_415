<?php
include_once 'mysql_op.php';
include_once 'error.php';
/*
 * Portocol:
 *       (1)  (2)                     (3)
 *     S 1.1:00 2.2:00 3.3:00 4.12:04
 *
 *     1 - reqest type.
 *     2 - descr.inter (timers).
 */
$deamon_fifo = "/tmp/fifo";
function send_daemon_rq($rq)
{
	
	$fd = @fopen($daemon_fifo, "w") or err_box("fopen ".$daemon_fifo, "stdf");
	@fwrite($fd, $rq) or err_box("fwrite ".$daemon_fifo, "stdf");
	
	return true;
}
function daemon_arm($shown_tt)
{
	$ring = $shown_tt['tt'];
	$size = count($ring);
	
	$rq = 'S';
	for( $i = 0; $i < $size; $i ++ ) {						
		$rq .= ' '.($i+1).'.'.$ring[$i+1];							
	}
	$rq .= "\n";
	send_daemon_rq($rq);
	return true;
}
?>
