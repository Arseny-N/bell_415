<?php
$prog = '/sbin/dcontrol';
function d_rexec()
{
	global $prog;
	system($prog.' rexec');
	return;	
}	
?>
