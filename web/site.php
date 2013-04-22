<?php
$sub_box_opened = false;
$main_box_opened = false;
function table_open($head,$cols)
{
	$numcols = count($cols);
	echo '
			<table class="main">
				<tr><th align="center" class="entry" colspan="'.$numcols.'"><div calss="table_heads">'.$head.'</div></td></tr>
				<tr>';
	$i = 0;
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
function print_navi_menu()
{
	echo '<table id="navi_box">';		  
	echo '<tr><td class="navi_box"> <a href="index.php" class="button" > Index </a></td></tr>';
	echo '<tr><td class="navi_box"> <a href="profs.php" class="button" > Profiles </a></td></tr>';
	echo '<tr><td class="navi_box"> <a href="logs.php" class="button" > Logs </a></td></tr>';
	echo '<tr><td class="navi_box"> <a href="help.php" class="button" > Help </a></td></tr>';
	echo '</table>';	
}
function print_head($style)
{
	echo '<html><head><meta charset="UTF-8"/><link type="text/css" rel="stylesheet" href="'.$style.'"></head>';
}
?>
