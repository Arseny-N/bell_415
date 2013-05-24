<?php

$sub_box_opened = false;
$main_box_opened = false;
function table_open($head,$cols=NULL, $args=NULL)
{
	$numcols = count($cols);

	echo '<table class="main" '.$args.'>';
	if($head) {		
		echo '<tr><th align="center" class="entry" colspan="'.$numcols.'"><div calss="table_heads">'.$head.'</th></tr>';
	}
	$i = 0;
	
	echo'<tr>';
	while ($numcols > $i)  {
		echo '<th align="center" class="entry">'.$cols[$i++].'</th>';
	}
	echo '</tr>';		
}
function table_close()
{
	echo '</table>';
}
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
function print_navi_menu($disable_logs = true)
{
	echo '<table id="navi_box">';		  
	echo '<tr><td class="navi_box"> <a href="profiles.php" class="button" > Профили </a></td></tr>';	
	if(!$disable_logs)
		echo '<tr><td class="navi_box"> <a href="logs.php" class="button" > Логи </a></td></tr>';
	echo '<tr><td class="navi_box"> <a href="help.php" class="button" > Помощь </a></td></tr>';
	echo '</table>';	
}
function print_head($style)
{

	echo '<html >
	<head>
		<meta charset="UTF-8"/>
		<link type="text/css" rel="stylesheet" href="'.$style.'">
		<link rel="icon" href="img/u-telnet.png" type="image/png">
		<title>Настройка Звонка</title>
	</head>';
	date_default_timezone_set('Europe/Moscow');
}
function form_open($s=NULL)
{
		global $argv;
		echo '<form action="'.$argv[0].'" method="post" style="display:inline">';		
		if($s) {
			echo $s;
			echo '</form>';
		}
}
function form_close()
{
		echo '</form>';
}
?>
