<?php
$sub_box_opened = false;
$main_box_opened = false;
function sub_box_open($td_args=NULL,$tr_args=NULL,$table_args=NULL)
{
	global $sub_box_opened;
	if(!$sub_box_opened) {
		echo '
			<table class="inboxer"'.$table_args.'>
			<tr'.$tr_args.'><td '.$td_args.'>			
			';
		$sub_box_opened = true;
	}
	
}
function sub_box_close()
{
	global $sub_box_opened;
	if($sub_box_opened) {
		echo '
		</td></tr>
		</table>
		';
		$sub_box_opened = false;
	}
}
function print_banner()
{
	echo '<br><div class="banner" align="left">With Great Power 
										 Comes Great Responsibility </div>';
}
function main_box_open()
{
	global $main_box_opened;
	if(!$main_box_opened) {
		echo '<table class="main_box"><tr><td class="main_box">';	
		$main_box_opened = true;
	}
}
function main_box_close()
{
	global $main_box_opened;
	if($main_box_opened) {
		echo '</td></tr></table>';
		$main_box_opened = false;
	}
}
function print_head($style)
{
	echo <<<_EOH_
	<html>
<head>
  <meta charset="UTF-8"/>
  <link type="text/css" rel="stylesheet" href="$style">
</head>
_EOH_;
}
?>
